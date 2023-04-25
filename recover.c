#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./filter [flag] infile outfile\n");
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    BYTE buffer[512] ;
    char filename[8] ;
    int file_writing = 0;
    int count = -1;
    FILE *img ;

    while (fread(buffer, 1, 512, input) == 512)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && ((buffer[3] & 0xf0) == 0xe0))
        {
            if (file_writing == 0)
            {
                count ++ ;
                sprintf(filename, "%03i.jpg", count);
                img = fopen(filename, "w");
                fwrite(&buffer, 512, 1, img);
                file_writing = 1;
            }
            else
            {
                fclose(img);
                count++;
                sprintf(filename, "%03i.jpg", count);
                img = fopen(filename, "w");
                fwrite(&buffer, 512, 1, img);

            }
        }
        else
        {
            if (file_writing == 1)
            {
                fwrite(&buffer, 512, 1, img);
            }
        }
    }

    fclose(input);
    fclose(img);
}