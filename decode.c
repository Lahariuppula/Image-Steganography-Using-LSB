#include <stdio.h>
#include "decode.h"
#include "types.h"
#include <string.h>

/* Function Definitions */

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if (argv[2] == NULL || argv[3] == NULL)
    {
        return e_failure;
    }
    if(strstr(argv[2],".bmp") != NULL)//beautiful.bmp
    {
        decInfo->stego_image_fname = argv[2];
    }
    else
    {
        return e_failure;
    }
    if(strstr(argv[3],".txt") != NULL)//decode.txt
    {
        decInfo->output_fname = argv[3];
    }
    else
    {
        return e_failure;
    }
    return e_success;
}

Status decode_open_files(DecodeInfo *decInfo)
{
    // Stego Image file
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    // Do Error handling
    if (decInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);

    	return e_failure;
    }

    // Output file
    decInfo->fptr_output = fopen(decInfo->output_fname, "w");
    // Do Error handling
    if (decInfo->fptr_output == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->output_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

/*Start the Decoding*/
Status start_decoding(DecodeInfo *decInfo)
{
    // Skip BMP header (54 bytes)
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);
    return e_success;
}

/* Perform the Decoding */
Status do_decoding(DecodeInfo *decInfo)
{
    if(decode_open_files(decInfo) == e_success)
    {
        printf("Files are opened successfully\n");
    }
    else
    {
        printf("Failed to open files\n");
        return e_failure;
    }
    if (start_decoding(decInfo) == e_success)
    {
        printf("File pointer moved to pixel data\n");
    }
    else
    {
        printf("Failed to move file pointer\n");
        return e_failure;
    }
    if (decode_magic_string(decInfo) == e_success)
    {
        printf("Magic string matched\n");
    }
    else
    {
        printf("Magic string mismatch\n");
        return e_failure;
    }
    if (decode_secret_file_extn_size(decInfo) == e_success)
    {
        printf("Decoding secret file extension size is a success\n");
    }
    else
    {
        printf("Decoding secret file extension size is a failure\n");
        return e_failure;
    }
    if (decode_secret_file_extn(decInfo) == e_success)
    {
        printf("Decoding secret file extension is a success\n");
    }
    else
    {
        printf("Decoding secret file extension is a failure\n");
        return e_failure;
    }
    if (decode_secret_file_size(decInfo) == e_success)
    {
        printf("Decoding secret file size is a success\n");
    }
    else
    {
        printf("Decoding secret file size is a failure\n");
        return e_failure;
    }
    if (decode_secret_file_data(decInfo) == e_success)
    {
        printf("Decoding secret file data is a success\n");
    }
    else
    {
        printf("Decoding secret file data is a failure\n");
        return e_failure;
    }
    return e_success;
}

/* Decode a byte into LSB of image data array */
Status decode_byte_to_lsb(char *data, char *image_buffer)
{
    char ch = 0;
    for (int i = 0; i < 8; i++)
    {
        ch = ch | ((image_buffer[i] & 1) << i);
    }
    *data = ch;
    return e_success;
}

/*Decode Magic string*/
Status decode_magic_string(DecodeInfo *decInfo)
{
    char decoded_magic[3];   // for "#*" + '\0'
    char buffer[8];

    for (int i = 0; i < 2; i++)
    {
        fread(buffer, 8, 1, decInfo->fptr_stego_image);
        decode_byte_to_lsb(&decoded_magic[i], buffer);
    }
    decoded_magic[2] = '\0';

    if (strcmp(decoded_magic, MAGIC_STRING) == 0)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    char buffer[32];
    int size = 0;
    // Read 32 bytes from stego image
    fread(buffer, 32, 1, decInfo->fptr_stego_image);
    // Extract 32 LSBs to form integer
    for (int i = 0; i < 32; i++)
    {
        size = size | ((buffer[i] & 1) << i);
    }
    // Store extension size
    decInfo->extn_size = size;
    return e_success;
}

Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char buffer[8];
    for (int i = 0; i < decInfo->extn_size; i++)
    {
        // Read 8 bytes from stego image
        fread(buffer, 8, 1, decInfo->fptr_stego_image);
        // Decode one character
        decode_byte_to_lsb(&decInfo->extn_secret_file[i], buffer);
    }
    // Null terminate extension string
    decInfo->extn_secret_file[decInfo->extn_size] = '\0';
    return e_success;
}

Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char buffer[32];
    long size = 0;
    // Read 32 bytes from stego image
    fread(buffer, 32, 1, decInfo->fptr_stego_image);
    // Extract 32 LSBs to form file size
    for (int i = 0; i < 32; i++)
    {
        size = size | ((long)(buffer[i] & 1) << i);
    }
    // Store decoded secret file size
    decInfo->size_secret_file = size;
    return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char buffer[8];
    char ch;
    for (long i = 0; i < decInfo->size_secret_file; i++)
    {
        // Read 8 bytes from stego image
        fread(buffer, 8, 1, decInfo->fptr_stego_image);
        // Decode one byte
        decode_byte_to_lsb(&ch, buffer);
        // Write decoded byte to output file
        fwrite(&ch, 1, 1, decInfo->fptr_output);
    }
    return e_success;
}