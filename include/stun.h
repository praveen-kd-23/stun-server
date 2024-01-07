#include <stdint.h>
#include <arpa/inet.h>
#include <xml.h>

#define CONF_FILE "conf/stun.xml"
#define STUN_MAGIC_COOKIE 0x2112A442

typedef struct stun_config {
   int port;
   char *host; // incase of multiple local interfaces
   char *software;
} stun_config_t;

typedef enum {
	STUN_REQUEST = 0x0001,
	STUN_RESPONSE = 0x0101,
} stun_msg_types_t;

typedef enum {
	STUN_TYPE_PACKET_TYPE,
	STUN_TYPE_ATTR,
	STUN_TYPE_ERROR
} stun_type_t;

typedef enum {
	STUN_ATTR_MAPPED_ADDRESS = 0x0001,
	STUN_ATTR_RESPONSE_ADDRESS = 0x0002,
	STUN_ATTR_XOR_MAPPED_ADDRESS = 0x0020,
    STUN_ATTR_SOFTWARE = 0x8022
} stun_attr_types_t;

typedef struct {
	uint8_t reserved;
	uint8_t family;
	uint16_t port;
	uint32_t address;
} stun_ip_t;

typedef struct {
	uint16_t type;
	uint16_t length;
	uint32_t cookie;
	char id[12];
} stun_header_t;

typedef struct {
	uint16_t type;
	uint16_t length;
	char value[];
} stun_attr_t;

typedef struct {
	stun_header_t header;
	uint8_t attr[];
} stun_packet_t;

void load_config(const char *file, stun_config_t *config);

stun_packet_t* stun_packet_parse(char *buffer, uint16_t len);
stun_packet_t* stun_packet_initialize(char *reply , char *id, stun_msg_types_t type);

void stun_packet_add_attr_xor_mapped_address(stun_packet_t *packet, char *ip, int port);
void stun_packet_add_attr_mapped_address(stun_packet_t *packet, char *ip, int port);
void stun_packet_add_attr_software(stun_packet_t *packet, char* software, uint16_t len);