#include <stun.h>

#define MAX_SIZE 513
stun_config_t config = {0};

void start_listener(){
    
    int sockfd;
    char buffer[MAX_SIZE];
    char reply[MAX_SIZE];
    int len = 0;
    socklen_t  addr_len = 0;
    struct sockaddr_in server_addr, client_addr; 

    stun_packet_t *packet = NULL;
    stun_packet_t *response = NULL;
    char client_ip[INET_ADDRSTRLEN];
    int client_port ;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    }

    memset(&server_addr, 0, sizeof(server_addr)); 
    memset(&client_addr, 0, sizeof(client_addr));

    server_addr.sin_family = AF_INET; 
    server_addr.sin_port = htons(config.port); 

    if(config.host && inet_pton(AF_INET, config.host, &server_addr.sin_addr) <= 0) {
        printf("Host Ip not configured , listening in default address\n");
        server_addr.sin_addr.s_addr = INADDR_ANY;
    }

    if(bind(sockfd, (const struct sockaddr *)&server_addr,  sizeof(server_addr)) < 0){ 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 

    addr_len = sizeof(client_addr);

    while(1){

        memset(buffer, '\0', sizeof(buffer));
        memset(reply, '\0', sizeof(reply));
        memset(client_ip, '\0', sizeof(client_ip));
        memset(&client_addr, '\0', sizeof(client_addr));
        
        len = recvfrom(sockfd, (char*)buffer, MAX_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len);

        if(len < 512){
          
           packet = stun_packet_parse(buffer, len);

           if(!packet){
             continue;
           }

           inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
           client_port = ntohs(client_addr.sin_port);

           response = stun_packet_initialize(reply , packet->header.id, STUN_RESPONSE);

           stun_packet_add_attr_xor_mapped_address(response, (char *) client_ip, client_port);
           stun_packet_add_attr_mapped_address(response, (char *) client_ip, client_port);
           stun_packet_add_attr_software(response, config.software, (uint16_t)strlen(config.software));

           sendto(sockfd, reply, ntohs(response->header.length)+sizeof(stun_header_t), 0, (struct sockaddr *)&client_addr, sizeof(client_addr)); 
        }

    }
}

void main(){
    
    load_config(CONF_FILE, &config);

    start_listener();

}