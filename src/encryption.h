#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <QDialog>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QInputDialog>
#include <openssl/rsa.h>
#include <openssl/engine.h>
#include <openssl/pem.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/aes.h>
#include <openssl/rand.h>

#define KEYSIZE 32
#define IVSIZE 32
#define BLOCKSIZE 256
#define SALTSIZE 8

namespace Ui {
class Encryption;
}

class Encryption : public QDialog
{
    Q_OBJECT

public:
    explicit Encryption(QWidget *parent = 0);
    ~Encryption();

private slots:
    void on_startBtn_clicked();

private:
    Ui::Encryption *ui;

    void panicMessage();
    QString changeFileName(QString filename);

    /**
     * @brief Encrypt a byte array with AES 256 CBC
     * @param The passphrase to encrypt the byte array data
     * @return QByteArray
     */
    QByteArray encryptAES(QByteArray passphrase, QByteArray &data);

    /**
     * @brief Decrypt a byte array with AES 256 CBC
     * @param The passphrase to decrypt the byte array data
     * @return QByteArray
     */
    QByteArray decryptAES(QByteArray passphrase, QByteArray &data);

    /**
     * @brief Get a byte array filled with random information
     * @param The size of the byte array to generate
     * @return QByteArray
     */
    QByteArray randomBytes(int size);

    /**
      * @brief Initalize the OpenSSL Lib and read/write the files to encrypt/decrypt
      */
    void initalize();

    /**
      * @brief Cleanup after the OpenSSL lib
      */
    void finalize();

    /**
     * @brief Loads a file and returns a byte array
     * @param The fiel to read from
     * @return QByteArray
     */
    QByteArray readFile(QString filename);

    /**
     * @brief Writes a byte array to a file
     * @param The name of the file to write to
     * @param The byte array to write
     */
    void writeFile(QString filename, QByteArray &data);

};

#endif // ENCRYPTION_H
