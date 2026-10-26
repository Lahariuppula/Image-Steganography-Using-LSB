#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include <string.h>

int main(int argc, char *argv[])
{
    EncodeInfo E1;
    DecodeInfo D1;
    //Validate CLA
    if (argc < 4 || argc > 5)
    {
        printf("Usage:\n");
        printf("For encoding : ./a.out -e beautiful.bmp secret.txt [stego.bmp]\n");
        printf("For decoding : ./a.out -d stego.bmp [decode.txt]\n");
    }
    int res = check_operation_type(argv);
    if(res == e_encode)
    {
        printf("Selected encoding\n");
        if(read_and_validate_encode_args(argv, &E1) == e_success)
        {
            printf("INFO : Read and validate encode_args is success\n");
            if(do_encoding(&E1) == e_success)
            {
                printf("Encoding is success\n");
            }
            else
            {
                printf("Failed to encode\n");
                return 0;
            }
        }
        else
        {
            printf("Error : Failed to read and validate encode_args\n");
            return 0;
        }
    }
    else if(res == e_decode)
    {
        printf("Selected decoding");
        if(read_and_validate_decode_args(argv, &D1) == e_success)
        {
            printf("\nINFO : Read and validate decode_args is success\n");
            if(do_decoding(&D1) == e_success)
            {
                printf("Decoding is success\n");
            }
            else
            {
                printf("Failed to decode\n");
                return 0;
            }
        }
    }
    else
    {
        printf("Error : Failed to read and validate decode_args\n");
        return 0;
    }
    return 0;
}
OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1] ,"-e") == 0)
    {
        return e_encode;
    }
    else if(strcmp(argv[1] ,"-d") == 0)
    {
        return e_decode;
    }
    else
    {
        //invalid
        return e_unsupported;
    }

}
