#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    if(strstr(argv[2],".bmp") != NULL)
    {
	encInfo->src_image_fname=argv[2];
    }
    else
    {
	printf("Error:please provide .bmp file to encode\n");
	e_failure;
    }


    if(strstr(argv[3],".txt")!=NULL)
    {
	encInfo->secret_fname=argv[3];
    }
    else
     {
	printf("Error:please provide .txt file to encode\n");
	e_failure;
    }


    
    if((argv[4]) != NULL)
    {
	if(strstr(argv[4],".bmp") != NULL)
	{
	    encInfo->stego_image_fname=argv[4];
	}
	else
	{
	    printf("Error:please provide .bmp file to encode\n");
	    e_failure;
	}
    }
    else
    {
	encInfo->stego_image_fname="output.bmp";
    }




    return e_success;
}


Status do_encoding(EncodeInfo *encInfo)
{
    if(open_files(encInfo) == e_success)
    {
	printf("INFO : Files are oepned successfully\n");
    }
    else
    {
	printf("INFO : Files are not opened successfully\n");
	return e_failure;
    }

    
    if(check_capacity(encInfo)==e_success)
    {
	printf("File size validate\n");
    }
    else
    {
	printf("File size not validate\n");
	return e_failure;
    }


    if(copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image)==e_success)
    {
	printf("Header copied successfully\n");
    }
    else
    {
	printf("Failed in copying Header\n");
	return e_failure;
    }


    if(encode_magic_string(MAGIC_STRING, encInfo) == e_success)
    {
	printf("Encode magic string executed successfully\n");
    }
    else
    {
	printf("encode magic string not executed successfully\n");
	return e_failure;
    }

    //encode_secret_file_extn_size

    if(encode_secret_file_extn_size(strlen(strchr(encInfo->secret_fname,'.')),encInfo)==e_success)
    {
	printf("Extension size encoded successfully\n");
    }
    else
    {
	printf("Extension size failed to encode\n");
	return e_failure;
    }

    //encode secret file extension
    if(encode_secret_file_extn(strchr(encInfo->secret_fname,'.'),encInfo)==e_success)
    {
	printf("secret file extn encoded\n");
    }
    else
    {
	printf("sorry...failed to encode secret file extn\n");
	return e_failure;
    }

    //encode secret file size
    if(encode_secret_file_size(get_file_size(encInfo->fptr_secret),encInfo)==e_success)
    {
	printf("File size encoded successfully\n");


    }
    else
    {
	printf("Failed encode file size\n");
	return e_failure;
    }

    //encode secret file data of encinfo

    if(encode_secret_file_data(encInfo)==e_success)
    {
	printf("File data encoded successfully\n");
    }
    else
    {
	printf("Failed to encode the file data\n");
	return e_failure;
    }

    //copy remaining data 

    if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
    {
	printf("successfully encoded remaining img data\n");
    }
    else
    {
	printf("Failed to encode remaining img data\n");
    }
    return e_success;
}




/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image); // it will read data 4b one tym from &width
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

//size of file
uint get_file_size(FILE *fptr)
{
    fseek(fptr,0,SEEK_END);
    int size=ftell(fptr);
    rewind(fptr);
    return size;
}

//encoding bmp header

Status copy_bmp_header(FILE *fptr_src_image,FILE *fptr_dest_image)
{
    rewind(fptr_src_image);
    char buffer[54];
    fread(buffer,54,1,fptr_src_image);
    fwrite(buffer,54,1,fptr_dest_image);

    if(ftell(fptr_dest_image)==54)
    {
	return e_success;
    }
    else
    {
	return e_failure;
    }
}

//encoding magic string

Status encode_magic_string(const char *magic_string,EncodeInfo *encInfo)
{
    if(encode_data_to_image((char*)magic_string,strlen(magic_string),encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
    {
	return e_success;
    }
    else
    {
	return e_failure;
    }
}

//encoding

Status encode_data_to_image(char *data,int size,FILE *fptr_src_image,FILE *fptr_stego_image)
{
    char arr[8];
    for(int i=0;i<size;i++)
    {
	fread(arr,8,1,fptr_src_image);
	encode_byte_to_lsb(data[i],arr);
	fwrite(arr,8,1,fptr_stego_image);
    }
    return e_success;
} 


Status encode_byte_to_lsb(char data,char *image_buffer)
{
    for(int i=0;i<8;i++)
    {
	image_buffer[i] = (image_buffer[i] & 0xfe) |((data>>i) & 0x01);
    }
}



//encoding size

Status encode_size_to_lsb(int len,EncodeInfo *encInfo)
{
    char buffer[32];
    fread(buffer,32,1,encInfo->fptr_src_image);
    for(int i=0;i<32;i++)
    {
	buffer[i] = (buffer[i] & 0xfe) | (len>>i & 0x01);
    }
    fwrite(buffer,32,1,encInfo->fptr_stego_image);
    return e_success;
}


//encoding secret file extn size
Status encode_secret_file_extn_size(int size,EncodeInfo *encInfo)
{
    if(encode_size_to_lsb(size,encInfo)==e_success)
    {
	return e_success;
    }
    else
    {
	return e_failure;
    }
}


//encode secret file extn
Status encode_secret_file_extn(const char *file_extn,EncodeInfo *encInfo)
{
    if(encode_data_to_image((char *) file_extn,strlen(file_extn),encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
    {
	return e_success;
    }
    else
    {
	return e_failure;
    }
}


//encode secret file size
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    if(encode_size_to_lsb(file_size,encInfo)==e_success)
    {
	return e_success;
    }
    else
    {
	return e_failure;
    }
}


//encode secret file data
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    int size=get_file_size(encInfo->fptr_secret);
    char buffer[size+1];
    rewind(encInfo->fptr_secret);
    fread(buffer,size,1,encInfo->fptr_secret);
    buffer[size]='\0';
    encode_data_to_image(buffer,size,encInfo->fptr_src_image,encInfo->fptr_stego_image);
    return e_success;
}


//copy remaining image data
Status copy_remaining_img_data(FILE *fptr_src,FILE *fptr_dest)
{
    char ch;
    while(fread(&ch,1,1,fptr_src)>0)
    {
	fputc(ch,fptr_dest);
    }
    return e_success;
}



/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

Status check_capacity(EncodeInfo *encInfo)
{
    if(get_image_size_for_bmp(encInfo->fptr_src_image) >= (54+strlen(MAGIC_STRING)*8+32+strlen(strchr(encInfo->secret_fname,'.'))*8+32))
    {
	return e_success;
    }
    else
    {
	return e_failure;
    }
}



