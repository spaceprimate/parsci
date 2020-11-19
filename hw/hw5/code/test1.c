#include <stdio.h>
#include <curl/curl.h>
#include "mpi.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
// to run:      mpicc -g test1.c -o tweet -lcurl
// to run:      mpirun -np 2 tweet


// todo: see the "next_token" at next_token":"b26v89c19zqg8o3fosbvxc1hiph7n5eruiuv2ccff908t"}



// helpful links:   https://developer.twitter.com/en/docs/twitter-api/v1/tweets/search/guides/standard-operators
//                  https://developer.twitter.com/en/docs/twitter-api/tweets/search/integrate/paginate
//                  https://developer.twitter.com/en/docs/twitter-api/tweets/search/quick-start


int function_pt(void *ptr, size_t size, size_t nmemb, void *stream){
    printf("%d", atoi(ptr));
    //printf("size: %d\n", (int)nmemb);
    return nmemb;
}

int main( int argc, char *argv[] )
{

    int rank, proc_size;
    MPI_Status status;
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &proc_size );

  CURL *curl;
  CURLcode res;
 
  curl = curl_easy_init();
  if(curl) {
    struct curl_slist *chunk = NULL;

    // char* hashtag = "nomasks";
    // char* base = "https://api.twitter.com/2/tweets/search/recent?query=%23nomasks";
    // char url[500];
    // strcpy(url, "https://api.twitter.com/2/tweets/search/recent?query=%23nomasks");
    // strcat(url, hashtag);

 
    /* Remove a header curl would otherwise add by itself */ 
    chunk = curl_slist_append(chunk, "Authorization: Bearer AAAAAAAAAAAAAAAAAAAAALRuCwEAAAAA9u01nsZ%2Fl3RJTYPQtD%2FHLYppQr8%3DMvcUErJQeGcaLkuO9j4iq5TsR1GIX6TUJc1cRKpiApWPR9ziqf");
    chunk = curl_slist_append(chunk, "-d {}");

    // char* url = "https://api.twitter.com/labs/2/tweets/1138505981460193280?expansions=attachments.media_keys&tweet.fields=created_at,author_id,lang,source,public_metrics,context_annotations,entities";
    // char* url = "https://api.twitter.com/labs/2/tweets/1138505981460193280?expansions=attachments.media_keys&tweet.fields=created_at,author_id,lang,source,public_metrics,context_annotations,entities";
    // char* url = "https://api.twitter.com/2/tweets/search/stream?tweet.fields=created_at&expansions=author_id&user.fields=created_at";
    // char* url = "https://api.twitter.com/2/tweets/search/recent?query=from:scalzi";
    // char* url = "https://api.twitter.com/2/tweets/search/recent?query=%23nomasks&next_token=b26v89c19zqg8o3fosbvxc1hiph7n5eruiuv2ccff908t";
    char* url;
    if(rank == 0){
        url = "https://api.twitter.com/2/tweets/search/recent?query=%23nomasks";
    }
    else{
        url = "https://api.twitter.com/2/tweets/search/recent?query=%23covid";
    }
    
    
    // char* url = "https://api.twitter.com/2/tweets/sample/stream";
 
    /* Add a custom header */ 
    // chunk = curl_slist_append(chunk, "Another: yes");
 
    // /* Modify a header curl otherwise adds differently */ 
    // chunk = curl_slist_append(chunk, "Host: example.com");
 
    // /* Add a header with "blank" contents to the right of the colon. Note that
    //    we're then using a semicolon in the string we pass to curl! */ 
    // chunk = curl_slist_append(chunk, "X-silly-header;");
 
    /* set our custom set of headers */ 
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
 
    // curl_easy_setopt(curl, CURLOPT_URL, "localhost");
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, function_pt); 
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
