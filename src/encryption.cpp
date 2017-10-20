#include "encryption.h"
#include "ui_encryption.h"

#include <QFileDialog>
#include <QInputDialog>

Encryption::Encryption(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Encryption)
{
    ui->setupUi(this);

    this->setFixedSize(400,300);

    ui->optionBox->addItem(tr("Encrypt"));
    ui->optionBox->addItem(tr("Decrypt"));

    ui->encryptionBox->addItem(tr("AES 256 bits"));

    initalize();

}

Encryption::~Encryption()
{
    finalize();

    delete ui;
}

void Encryption::initalize()
{
    ERR_load_CRYPTO_strings();
    OpenSSL_add_all_algorithms();
    OPENSSL_config(NULL);
}

void Encryption::finalize()
{
    EVP_cleanup();
    ERR_free_strings();
}

QByteArray Encryption::readFile(QString filename)
{
    QByteArray data;
    QFile file(filename);
    if(!file.open(QFile::ReadOnly))
    {
        QMessageBox::warning(this,tr("Error: Open file"),tr("Could not open file to read: %1").arg(file.errorString()));
        return data;
    }

    data = file.readAll();
    file.close();
    return data;
}

void Encryption::writeFile(QString filename, QByteArray &data)
{
    QFile file(filename);
    if(!file.open(QFile::WriteOnly))
    {
        QMessageBox::warning(this,tr("Error: Open file"),tr("Could not open file to write: %1").arg(file.errorString()));
        return;
    }

    file.write(data);
    file.close();
}


QByteArray Encryption::encryptAES(QByteArray passphrase, QByteArray &data)
{
    QByteArray msalt = randomBytes(SALTSIZE);
    int rounds = 1;
    unsigned char key[KEYSIZE];
    unsigned char iv[IVSIZE];

    const unsigned char* salt = (const unsigned char*) msalt.constData();
    const unsigned char* password = (const unsigned char*) passphrase.constData();

    int i = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), salt,password, passphrase.length(),rounds,key,iv);

    if(i != KEYSIZE)
    {
        qCritical() << "EVP_BytesToKey() error: " << ERR_error_string(ERR_get_error(), NULL);

        panicMessage();

        exit(1);
    }

    EVP_CIPHER_CTX *en = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(en);

    if(!EVP_EncryptInit_ex(en, EVP_aes_256_cbc(),NULL,key, iv))
    {
        panicMessage();

        exit(2);
    }

    char *input = data.data();
    int len = data.size();

    int c_len = len + AES_BLOCK_SIZE, f_len = 0;
    unsigned char *ciphertext = (unsigned char*)malloc(c_len);

    if(!EVP_EncryptInit_ex(en, NULL, NULL, NULL, NULL))
    {
        panicMessage();

        exit(3);
    }

    // May have to repeat this for large files

    if(!EVP_EncryptUpdate(en, ciphertext, &c_len,(unsigned char *)input, len))
    {
        panicMessage();

        exit(4);
    }

    if(!EVP_EncryptFinal(en, ciphertext+c_len, &f_len))
    {
        panicMessage();

        exit(5);
    }

    len = c_len + f_len;
    EVP_CIPHER_CTX_cipher(en);

    //ciphertext

    QByteArray encrypted = QByteArray(reinterpret_cast<char*>(ciphertext), len);
    QByteArray finished;
    finished.append("Salted__");
    finished.append(msalt);
    finished.append(encrypted);

    free(ciphertext);

    EVP_CIPHER_CTX_free(en);

    return finished;
}

QByteArray Encryption::decryptAES(QByteArray passphrase, QByteArray &data)
{
    QByteArray msalt;
    if(QString(data.mid(0,8)) == "Salted__")
    {
        msalt = data.mid(8,8);
        data = data.mid(16);
    }
    else
    {
        QMessageBox::warning(this,tr("Error: Decrypting"),tr("Clevit could not decrypt the file,\n"
                                                             "because the file aren't encrypted "
                                                             "or was not well encrypted"));

        msalt = randomBytes(SALTSIZE);
    }

    int rounds = 1;
    unsigned char key[KEYSIZE];
    unsigned char iv[IVSIZE];
    const unsigned char* salt = (const unsigned char*)msalt.constData();
    const unsigned char* password = (const unsigned char*)passphrase.data();

    int i = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), salt,password, passphrase.length(),rounds,key,iv);

    if(i != KEYSIZE)
    {
        panicMessage();

        exit(1);
    }

    EVP_CIPHER_CTX *de = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(de);

    if(!EVP_DecryptInit_ex(de,EVP_aes_256_cbc(), NULL, key,iv ))
    {
        panicMessage();

        exit(2);
    }

    char *input = data.data();
    int len = data.size();

    int p_len = len, f_len = 0;
    unsigned char *plaintext = (unsigned char *)malloc(p_len + AES_BLOCK_SIZE);

    //May have to do this multiple times for large data???
    if(!EVP_DecryptUpdate(de, plaintext, &p_len, (unsigned char *)input, len))
    {
        panicMessage();

        exit(3);
    }

    if(!EVP_DecryptFinal_ex(de,plaintext+p_len,&f_len))
    {
        QMessageBox::information(this,tr("Wrong passphrase"),tr("Probably your passphrase are wrong.\n"
                                                                "Clevit will be closed for safety.\n"
                                                                "YOUR DATA HAS NOT BEEN AFFECTED."));

        exit(4);
    }

    len = p_len + f_len;

    EVP_CIPHER_CTX_cleanup(de);


    QByteArray decrypted = QByteArray(reinterpret_cast<char*>(plaintext), len);

    free(plaintext);

    EVP_CIPHER_CTX_free(de);

    return decrypted;

}

QByteArray Encryption::randomBytes(int size)
{
    unsigned char arr[size];
    RAND_bytes(arr,size);

    QByteArray buffer = QByteArray(reinterpret_cast<char*>(arr), size);
    return buffer;
}

void Encryption::on_startBtn_clicked()
{
    if(ui->optionBox->currentIndex() == 0)
    {
        QString filePath = QFileDialog::getOpenFileName(this,tr("Select a text file"),QDir::currentPath(),tr("All Files (*.*);;Text Files (*.txt);"));

        qDebug() << QFileInfo(filePath).suffix();

        if(QString::compare(filePath,"") == 0)
        {
            QMessageBox::warning(this,tr("Error: Selecting File"),tr("Invalid Path File"));
            return;
        }

        QByteArray data = readFile(filePath);

        QByteArray passphrase = QInputDialog::getText(this,tr("Passphrase"),tr("Insert a passphrase"),QLineEdit::Password).toLocal8Bit();

        if(QString::compare(passphrase,"") == 0)
        {
            QMessageBox::warning(this,tr("Error: Creating passphrase"),tr("Invalid Passphrase. Try again!"));
            return;
        }

        QByteArray encrypted = encryptAES(passphrase,data);

        writeFile(changeFileName(filePath),encrypted);

        QMessageBox::information(this,tr("Encrypted File"),tr("File has been Encrypted!"));
    }
    else
        if(ui->optionBox->currentIndex() == 1)
        {
            QString filePath = QFileDialog::getOpenFileName(this,tr("Select a text file"),QDir::currentPath(),tr("All Files (*.*);;Text Files (*.txt);"));

            if(QString::compare(filePath,"") == 0)
            {
                QMessageBox::warning(this,tr("Error: Selecting File"),tr("Invalid Path File"));
                return;
            }

            QByteArray data = readFile(filePath);

            QByteArray passphrase = QInputDialog::getText(this,tr("Passphrase"),tr("Insert your passphrase"),QLineEdit::Password).toLocal8Bit();

            if(QString::compare(passphrase,"") == 0)
            {
                QMessageBox::warning(this,tr("Error: Inserting passphrase"),tr("Invalid Passphrase. Try again!"));
                return;
            }


            QByteArray decrypted = decryptAES(passphrase,data);

            writeFile(filePath,decrypted);

            QMessageBox::information(this,tr("Decrypted File"),tr("File has been Decrypted!"));
        }
}

QString Encryption::changeFileName(QString filename)
{
    QString ext = QFileInfo(filename).suffix();

    filename.truncate(filename.lastIndexOf('.'));

    filename.append("Encrypted");
    filename.append(".");
    filename.append(ext);

    return filename;
}

void Encryption::panicMessage()
{
    QMessageBox::information(this,tr("Wrong passphrase"),tr("An error occured.\n"
                                                            "Clevit will be closed for safety.\n"
                                                            "YOUR DATA HAS NOT BEEN AFFECTED."));
}
