#include <stdio.h>
#include "encode.h"
#include "types.h"
#include<string.h>
#include "decode.h"

int main(int argc, char *argv[])
{
    int type=check_operation_type(argv);
    if(type == e_encode)
    {
	//start encoding part
	EncodeInfo encInfo;
	if(argc>=4)
	{
	    if(read_and_validate_encode_args(argv, &encInfo) == e_success)
	    {
		if(do_encoding(&encInfo)==e_success)
		{
		    printf("encoded successfully\n");
		}
	    }
	}
	else
	{
	    printf("Please provide more than 3 arguments to encode\n");
	}
    }
    else if(type == e_decode)
    {
	//start decoding part
	DecodeInfo decInfo;
	if(argc>=3)
	{
	    if(read_and_validate_decode_args(argv,&decInfo)==d_success)
	    {
		printf("Read and validate completed for decoding\n");
		if(do_decoding(&decInfo)==d_success)
		{
		    printf("decoding successfully completed!!!\n");
		}
	    }
	}
	else
	{
	    printf("please provide more than 3 arguments to encode\n");
	}
    }
    else
    {
	//printf error msg
	printf("Please provide the opearation type\n");
    }
 }
OperationType check_operation_type(char *argv[])
{
    if(!strcmp(argv[1],"-e"))
    {
	return e_encode;
    }
   else if(!strcmp(argv[1],"-d"))
    {
	return e_decode;
    }
    else
    {
	return e_unsupported;
    }
}

