#include <openssl/evp.h>
#include <openssl/aes.h>
#include <cctype>
#include <string.h>
#include <openssl/evp.h>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
int base64Decode(unsigned char* In, int inLen, unsigned char* Out, int* pOutLen)

{
  EVP_ENCODE_CTX dctx;
  int inl,  i,  ret ;
  EVP_DecodeInit(&dctx);
  int OutL = 0;
  *pOutLen = 0;
  ret = EVP_DecodeUpdate(&dctx, Out, &OutL, In, inLen);
  if (ret < 0)
  {
    std::printf("EVP_DecodeUpdate err!\n");
    return -1;
  }
  *pOutLen += OutL;
  ret = EVP_DecodeFinal(&dctx, Out, &OutL);
  *pOutLen += OutL;
  return 0;
}

enum
{
  Base64TextMaxLen = 64,
  KeyLen = 32,
  PlainTextMaxlen = 128,
  IVLen=16
};
int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
  unsigned char *plaintext)
{
  EVP_CIPHER_CTX *ctx;
  unsigned char iv[IVLen];
  int len;

  int plaintext_len;
  std::memset(iv, 0, sizeof(iv));

  /* Create and initialise the context */
  if (!(ctx = EVP_CIPHER_CTX_new())) return 0;

  /* Initialise the decryption operation. IMPORTANT - ensure you use a key
  * and IV size appropriate for your cipher
  * In this example we are using 256 bit AES (i.e. a 256 bit key). The
  * IV size for *most* modes is the same as the block size. For AES this
  * is 128 bits */
  if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
    return -1;

  /* Provide the message to be decrypted, and obtain the plaintext output.
  * EVP_DecryptUpdate can be called multiple times if necessary
  */
  if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
    return -1;
  plaintext_len = len;

  /* Finalise the decryption. Further plaintext bytes may be written at
  * this stage.
  */
  if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) return -1;
  plaintext_len += len;

  /* Clean up */
  EVP_CIPHER_CTX_free(ctx);

  return plaintext_len;
}

int getPrice(const char* base64txt, const char* k ,  unsigned char * plainText)
{
  int plainLen = 0;
  unsigned char base64text[Base64TextMaxLen];
  unsigned char key[KeyLen];
  std::memset(key, 0, sizeof(key));
  unsigned char cipher[Base64TextMaxLen];
  int base64textlen, cipherlen;
  int len;
  std::snprintf((char*)base64text, sizeof(base64text), "%s\n%n", base64txt, &base64textlen);
  std::memcpy(key, k, std::max(static_cast<std::size_t>(KeyLen), strlen(k)));
  base64Decode(base64text, base64textlen, cipher, &cipherlen);
  plainLen = decrypt(cipher, cipherlen, key, plainText);
  plainText[plainLen] = 0;
  return plainLen;
}

int main(int argc, char** argv)
{
  if (argc<3)
  {
    std::printf("usage: ./a.out base64Str Token");
    return 1;
  }
  unsigned char plainText[PlainTextMaxlen];
  int plainlen = getPrice(argv[1], argv[2], plainText);
  plainText[plainlen] = 0;
  std::cout << plainText<<std::endl;
}


