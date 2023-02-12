#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int server_sample(const u_short port);
void set_recv_timeout(const int socket, const u_long milli_sec);

int main()
{
    server_sample(12346);

    return 0;
}

int server_sample(const u_short port)
{
    int sock0;
    struct sockaddr_in addr;
    struct sockaddr_in client;
    int len, len2;
    int sock;

    sock0 = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(sock0, (struct sockaddr *)&addr, sizeof(addr));
    listen(sock0, 5);
    
    for ( ; ; )
    {
        char buf[256];
        len = sizeof(client);
        sock = accept(sock0, (struct sockaddr *)&client, &len);
        set_recv_timeout(sock, 5000);
        write(sock, "HELLO\n", 6);
        for ( ; ;)
        {
            u_char data;
            len2 = read(sock, &data, sizeof(data));
            if (len2 < 0)
            {
                printf("disconnected\n");
                close(sock);
                break;
            }
            printf("%c", data);
        }
    }

    close(sock0);

    return 0;
}

void set_recv_timeout(const int socket, const u_long milli_sec)
{
    struct timeval tv;

    tv.tv_sec = milli_sec / 1000;
    tv.tv_usec = (milli_sec % 1000) * 1000;
    setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv));
}
