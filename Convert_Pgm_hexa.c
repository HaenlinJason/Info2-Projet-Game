#include <stdio.h>
#include <stdlib.h>
/*
permet d'avoir la bonne expression pour simplement le copier coller dans scilab pour 
avoir le résultat en hexa
home
    Image_Origine = fopen("F:\\VPN_IUT\\GEII S2\\ER2\\Code C\\LittleHearth.pgm", "r");
    Image_Nouvelle = fopen("F:\\VPN_IUT\\GEII S2\\ER2\\Code C\\LittleHearth.txt", "w+");
school
    Image_Origine = fopen("Z:\\GEII S2\\ER2\\Code C\\pac1.pgm", "r");
    Image_Nouvelle = fopen("Z:\\GEII S2\\ER2\\Code C\\pac1.txt", "w+");
*/
#define CONVERT
#ifdef CONVERT

int main()
{
    printf("Debut convertion\n");
    FILE *Image_Origine = NULL,*Image_Nouvelle= NULL;
    int j,i,WordSize,NbrWord;
    int *tabtake=NULL;
    char Tab[100];
    Image_Origine = fopen("F:\\VPN_IUT\\GEII S2\\INFO2\\Projet Info2\\image\\10.pgm", "r");
    Image_Nouvelle = fopen("F:\\VPN_IUT\\GEII S2\\INFO2\\Projet Info2\\image\\10.txt", "w+");
    //VERIFICATION DE L'OUVERTURE DES FICHIERS
    if ((Image_Origine != NULL)&&(Image_Nouvelle != NULL))
    {
        for (i=1; i<3; i++)
        {
            fgets(Tab,100,Image_Origine);
            fputs(Tab,Image_Nouvelle);
        }
        //FIRST PART
        fscanf(Image_Origine,"%d%d",&WordSize,&NbrWord);
        printf("\nWordSize   > %d\nNbrWord    > %d\n ",WordSize,NbrWord);
        fprintf(Image_Nouvelle,"\nWIDTH=%d;\nDEPTH=%d;\n\n",WordSize,NbrWord);
        fprintf(Image_Nouvelle,"ADDRESS_RADIX=UNS;\nDATA_RADIX=BIN;\n\n");
        fprintf(Image_Nouvelle,"CONTENT BEGIN\n");
        tabtake=calloc(WordSize,sizeof(int));
        if(tabtake!=NULL)
        {
            puts("Allocation memoire OK !");
        }
        int tabmemo[WordSize];
        //SCAN LIGNE (SCAN LIGNE PAR LIGNE)
        for(i=0; i<NbrWord; i++)
        {
            fprintf(Image_Nouvelle,"\ty = bin2dec(' ");
            for(j=0; j<WordSize; j++)
            {
                fscanf(Image_Origine,"%d",&tabtake[j]);
                //fprintf(Image_Nouvelle,"%d ",tabtake[j]);
            }
            for(j=0; j<WordSize; j++)
            {
                if(tabtake[j]==255)
                {
                    tabmemo[j]=0;
                }
                else
                {
                    tabmemo[j]=1;
                }
                fprintf(Image_Nouvelle,"%d",tabmemo[j]);
            }
            fprintf(Image_Nouvelle,"');\n");
            fprintf(Image_Nouvelle,"dec2hex(y)\n");
        }
        fprintf(Image_Nouvelle,"END;");
        //END SCAN FICHIER
        //PUIS ON FERME LES FICHIERS
        free(tabtake);
        fclose(Image_Origine);
        fclose(Image_Nouvelle);
    }
    else
    {
        // ON AFFICHE MESSAGE D'ERREUR SI PROBLEME OUVERTURE FICHIER
        printf("Impossible d'ouvrir une (ou les deux) image (s)\n");
    }
    printf("Fin convertion\n");
    return 0;
}
#endif //CONVERT
