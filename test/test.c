#include <stdio.h>
#include "../include/requests.h"
#include "greatest.h"

#ifdef _DEBUG_
# define DEBUG(M, ...) fprintf(stderr, "DEBUG %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
# define DEBUG(M, ...)
#endif

void test_print(REQ *req)
{
    printf("Request URL: %s\n", req->url);
    printf("Response Code: %lu\n", req->code);
    printf("Response Size: %zu\n", req->size);
    printf("Response Body:\n%s\n", req->text);
}

TEST get()
{
    long code = 200;
    size_t size = 1270;

    REQ req;
    CURL *curl = requests_init(&req, "http://example.com");
    requests_get(curl, &req);

    ASSERT_EQ(code, req.code);
    ASSERT_EQ(size, req.size);

    requests_close(curl, &req);
    PASS();
}

TEST post()
{
    long code = 200;
    char *data[] = {
        "apple", "red",
        "banana", "yellow"
    };
    int data_size = sizeof(data)/sizeof(char*);

    REQ req;
    CURL *curl = requests_init(&req, "http://www.posttestserver.com/post.php");
    char *body = requests_url_encode(curl, data, data_size);
    requests_post(curl, &req, body);

    ASSERT_EQ(code, req.code);

    curl_free(body);
    requests_close(curl, &req);
    PASS();
}

TEST post_nodata()
{
    long code = 200;

    REQ req;
    CURL *curl = requests_init(&req, "http://www.posttestserver.com/post.php");
    requests_post(curl, &req, NULL);

    ASSERT_EQ(code, req.code);

    requests_close(curl, &req);
    PASS();
}

TEST post_headers()
{
    long code = 200;
    char *headers[] = {
        "Content-Type: application/json",
        "Content-Hype: dude"
    };
    int headers_size = sizeof(headers)/sizeof(char*);

    REQ req;
    CURL *curl = requests_init(&req, "http://www.posttestserver.com/post.php");
    requests_post_headers(curl, &req, NULL, headers, headers_size);

    ASSERT_EQ(code, req.code);

    requests_close(curl, &req);
    PASS();
}

TEST put()
{
    long code = 200;
    char *data[] = {
        "apple", "red",
        "banana", "yellow"
    };
    int data_size = sizeof(data)/sizeof(char*);

    REQ req;
    CURL *curl = requests_init(&req, "http://www.posttestserver.com/post.php");
    char *body = requests_url_encode(curl, data, data_size);
    requests_put(curl, &req, body);

    ASSERT_EQ(code, req.code);

    curl_free(body);
    requests_close(curl, &req);
    PASS();
}

TEST urlencode()
{
    CURL *curl = curl_easy_init();
    char *data[] = {
        "apple", "red",
        "banana", "yellow"
    };
    int data_size = sizeof(data)/sizeof(char*);
    char *ideal = "apple%3Dred%26banana%3Dyellow";
    char *test = requests_url_encode(curl, data, data_size);

    ASSERT(strcmp(ideal, test) == 0);

    curl_free(test);
    curl_easy_cleanup(curl);

    PASS();
}

SUITE(tests)
{
    RUN_TEST(get);
    RUN_TEST(post);
    RUN_TEST(post_nodata);
    RUN_TEST(post_headers);
    RUN_TEST(put);
    RUN_TEST(urlencode);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(tests);
    GREATEST_MAIN_END();
    return 0;
}
