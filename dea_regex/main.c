#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dea.h"

#include "adjlist2dea.h"


int main( int argc, const char* argv[] )
{
    printf("%d\n", argc);
    if ( argc > 2 )
    {
        int i;
        for (i=0;i<strlen(argv[1]);i++)
            printf("%c", argv[1][i]);
        printf("\n");

        /* check that the given dea is valid adj list syntax */
        if ( is_adj_list_valid( argv[1] ) == ACCEPTING )
        { 
            dea_t* d = dea_from_adj_list( argv[1] );
            print_dea( d );

            printf("input is %s\n", ( verify_input( d, argv[2] ) != NOT_ACCEPTING ? "accepted" : "not accepted" ) );

            free_dea( d );
        }
        else
        {
            printf("invalid dea adj list\n");
        }
    }

    return 0;
}
