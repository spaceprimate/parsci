#include <stdio.h>
#include <curl/curl.h>
#include "mpi.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>


// todo: see the "next_token" at next_token":"b26v89c19zqg8o3fosbvxc1hiph7n5eruiuv2ccff908t"}

int function_pt(void *ptr, size_t size, size_t nmemb, void *stream){
    printf("%d", atoi(ptr));
    //printf("size: %d\n", (int)nmemb);
    return nmemb;
}


size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
        size_t written = fwrite(ptr, size, nmemb, stream);
            return written;
}

int main( int argc, char *argv[] )
{

    int rank, proc_size;
    MPI_Status status;
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &proc_size );
  
    FILE *fp;

  CURL *curl;
  CURLcode res;
 
  curl = curl_easy_init();
  if(curl) {
    struct curl_slist *chunk = NULL;


 
    /* Remove a header curl would otherwise add by itself */ 
    chunk = curl_slist_append(chunk, "Authorization: Bearer AAAAAAAAAAAAAAAAAAAAALRuCwEAAAAA9u01nsZ%2Fl3RJTYPQtD%2FHLYppQr8%3DMvcUErJQeGcaLkuO9j4iq5TsR1GIX6TUJc1cRKpiApWPR9ziqf");
    chunk = curl_slist_append(chunk, "-d {}");

    char* url;
    if(rank == 0){
        url = "https://api.twitter.com/2/tweets/search/recent?query=%23nomasks";
    }
    else if(rank == 1){
        url = "https://api.twitter.com/2/tweets/search/recent?query=%23covid";
    }
    else if (rank == 2){
   
        url = "https://api.twitter.com/2/tweets/search/recent?query=%23fauci";
    }
     else if (rank == 3){
   
        url = "https://api.twitter.com/2/tweets/search/recent?query=%23maskup";
    }
     
    /* set our custom set of headers */ 
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
 
    // curl_easy_setopt(curl, CURLOPT_URL, "localhost");
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
   // curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); 
    // curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp); 
    res = curl_easy_perform(curl);
    /* Check for errors */ 
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
 
    /* always cleanup */ 
    curl_easy_cleanup(curl);
 
    /* free the custom headers */ 
    curl_slist_free_all(chunk);
  }


    MPI_Finalize();



  return 0;
}
