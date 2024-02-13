#include<stdio.h>
#include "types.h"

//structure for decode
typedef struct DecodeInfo
{
    //stego image info
    char *encoded_image_fname;
    FILE *fptr_encoded_image;

    //secret file info
    char *output_fname;
    FILE *fptr_output;
    char extn_secret_file[4];
    long extn_size;
    long output_file_size;

}DecodeInfo;

//Read and validate decode args from argv
Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo);

//perform the decoding
Status do_decoding(DecodeInfo *decInfo);

//get file pointer i/p
Status open_decode_bmp_file(DecodeInfo *decInfo);

//decodes magic string
Status decode_magic_string(const char *magic_string,DecodeInfo *decInfo);

//decode file_extn_size
Status decode_file_ext_size(DecodeInfo *decInfo);

//decodes file_extn
Status decode_file_extn(DecodeInfo *decInfo);

//decode file size
Status decode_file_size(DecodeInfo *decInfo);

//decode file data
Status decode_file_data(DecodeInfo *decInfo);

//decode file from image
Status decode_file_from_image(int size,FILE *fptr_encoded_image,FILE *fptr_output);

//decodes byte from lsb
Status decode_byte_from_lsb(char *data,char *image_buffer);

//decode size from lsb
Status decode_size_from_lsb(FILE *fptr_encoded_image);








