#include <stun.h>

void callback(const char *key, const char *value, void *user_data){
     
     stun_config_t *config = (stun_config_t*)user_data;
     
     if(strcmp(key, "port") == 0){
        config->port = atoi(value);
     }
     else if(strcmp(key, "host") == 0){
        config->host = strdup(value);
     }
     else if(strcmp(key, "software") == 0){
        config->software = strdup(value);
     }
}

void load_config(const char *file, stun_config_t *config){
    XML *root = xml_parse(file);
    xml_iterate_key_value_pairs(root, callback, config);
}