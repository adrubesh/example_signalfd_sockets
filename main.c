#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/signalfd.h>
#include <unistd.h>


int
main (int argc, char **argv)
{
        /* refer to signalfd_example for explanations on signalfd calls */
        int sfd, sockfd;
        sigset_t mask;
        size_t s;
        struct signalfd_siginfo fdsi;
        struct sockaddr_in addr;

        sigemptyset(&mask);
        sigaddset(&mask, SIGINT);
        sigaddset(&mask, SIGQUIT);

        assert(sigprocmask(SIG_BLOCK, &mask, NULL) != -1);

        assert((sfd = signalfd(-1, &mask, 0)) != -1);

        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(43443);

        assert(bind(sockfd, (struct sockaddr *)&addr, sizeof(addr) == 0));
        
        assert(listen(sockfd, 1024) == 0);

        while(1) {
                s = read(sfd, &fdsi, sizeof(struct signalfd_siginfo));
                assert(s == sizeof(struct signalfd_siginfo));

                if (fdsi.ssi_signo == SIGINT) {
                        printf("INT!\n");
                        break;
                } else if (fdsi.ssi_signo == SIGQUIT) {
                        printf("QUIT!\n");
                        break;
                }
        }

        exit(0);

}
