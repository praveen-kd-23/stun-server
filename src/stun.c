#include <stun.h>

stun_packet_t* stun_packet_parse(char *buffer, uint16_t len){

    stun_packet_t *packet = (stun_packet_t*)buffer;

    packet->header.type = ntohs(packet->header.type);
    packet->header.length = ntohs(packet->header.length);

    return packet;
}

stun_packet_t* stun_packet_initialize(char *buffer , char *id, stun_msg_types_t type){

    stun_header_t *header = (stun_header_t*)buffer;

    header->type = htons(type);
    header->length = 0;
    header->cookie = htonl(STUN_MAGIC_COOKIE);

    memcpy(header->id, id, 12);

    return (stun_packet_t*)buffer;
}

void stun_packet_add_attr_xor_mapped_address(stun_packet_t *packet, char *client_ip, int port ){
    stun_attr_t *attr;
    stun_ip_t *ip;

	attr = (stun_attr_t*) ((uint8_t *) & packet->attr + ntohs(packet->header.length));
	attr->type = htons(STUN_ATTR_XOR_MAPPED_ADDRESS);

	attr->length = htons(8);

	ip = (stun_ip_t *) attr->value;

	ip->port = htons(port ^ (STUN_MAGIC_COOKIE >> 16));
        ip->family = 1;

	inet_pton(AF_INET, client_ip, (int *) &ip->address);
	ip->address = htonl(ntohl(ip->address) ^ STUN_MAGIC_COOKIE);

	packet->header.length += htons(sizeof(stun_attr_t)) + attr->length;
}

void stun_packet_add_attr_mapped_address(stun_packet_t *packet, char *client_ip, int port){
        stun_attr_t *attr;
	stun_ip_t *ip;

	attr = (stun_attr_t*) ((uint8_t *) & packet->attr + ntohs(packet->header.length));
	attr->type = htons(STUN_ATTR_MAPPED_ADDRESS);

	attr->length = htons(8);

	ip = (stun_ip_t *) attr->value;

	ip->port = port;
        ip->family = 1;

	inet_pton(AF_INET, client_ip, (int *) &ip->address);

	packet->header.length += htons(sizeof(stun_attr_t)) + attr->length;
}

void stun_packet_add_attr_software(stun_packet_t *packet, char* software, uint16_t len){
    stun_attr_t *attr;

    attr = (stun_attr_t*) ((uint8_t *) & packet->attr + ntohs(packet->header.length));
    attr->type = htons(STUN_ATTR_SOFTWARE);

    attr->length = htons(len);

    strncpy(attr->value, software, len);

    packet->header.length += htons(sizeof(stun_attr_t)) + attr->length;

}
