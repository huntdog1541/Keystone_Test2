#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <keystone/keystone.h>

ks_engine *ks;
ks_err err;
size_t size;
size_t count;

static void clearContents(char * contents, size_t size);
static void printValues(unsigned char * encoded);

static unsigned char* get_encoded(const char * assembly)
{
    unsigned char * encode;

    if(ks != NULL)
    {
        if(ks_asm(ks, assembly, 0, &encode, &size, &count))
        {
            printf("ERROR: failed on ks_asm() with count = %lu, error code=%u, with %s \n", count, ks_errno(ks), assembly);
            return NULL;
        }
        else
        {
            return encode;
        }
    }
}

static void removeEncode(unsigned char * encode)
{
    ks_free(encode);
}

static void close_ks()
{
    ks_close(ks);
}

static int open_ks(ks_arch arch, int mode, int syntax)
{
    err = ks_open(arch, mode, &ks);
    if(err != KS_ERR_OK)
    {
        printf("ERROR: failed on ks_open(), quit!\n");
        return -1;
    }

    if(syntax)
        ks_option(ks, KS_OPT_SYNTAX, syntax);
}

static char * getFileContents(char* fileName)
{
    printf("File: %s\n", fileName);
    FILE *fin = fopen(fileName, "r");
    if(fin == NULL)
    {
        perror("Unable to open input file!");
        return NULL;
    }
    fseek(fin, 0, SEEK_END);
    size_t fsize = ftell(fin);
    fseek(fin, 0, SEEK_SET);

    char * contents = malloc(fsize + 1);
    clearContents(contents, fsize);
    fread(contents, 1, fsize, fin);
    fclose(fin);

    return contents;
}

static void getOutput(char * outputFile, char * inputContent)
{
    FILE * fout = fopen(outputFile, "w");
    if(fout == NULL)
    {
        perror("Unable to open output file");
        return;
    }
    unsigned char * encoded = get_encoded(inputContent);
    fwrite(encoded, sizeof(char), size, fout);
    fclose(fout);
}

static void clearContents(char * contents, size_t size)
{
    size_t i = 0;
    for(; i < size; i++)
    {
        contents[i] = '\0';
    }
}

int main() {

    open_ks(KS_ARCH_X86, KS_MODE_64, KS_OPT_SYNTAX_INTEL);
    char * fileContents = getFileContents("test.txt");
    getOutput("test.bin", fileContents);
    return 0;
}