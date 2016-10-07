#include "../include/util/common-header.h"
#include "../include/proxy/proxy.h"
#include <libconfig.h>


int proxy_read_config(struct proxy_node_t* cur_node,const char* config_path){
    config_t config_file;
    config_init(&config_file);

    if(!config_read_file(&config_file,config_path)){
        goto goto_config_error;
    }
    
    uint8_t group_size;
    if(!config_lookup_int(&config_file,"group_size",(int*)&group_size)){
        goto goto_config_error;
    }

    if(group_size<=cur_node->node_id){
        err_log("PROXY : Invalid Node Id\n");
        goto goto_config_error;
    }
    cur_node->group_size = group_size;

// parse proxy address

    config_setting_t *proxy_config = NULL;
    proxy_config = config_lookup(&config_file,"proxy_config");

    if(NULL==proxy_config){
        err_log("PROXY : Cannot Find Nodes Settings.\n");
        goto goto_config_error;
    }    

    config_setting_t *pro_ele = config_setting_get_elem(proxy_config,cur_node->node_id);

//    err_log("PROXY : Current Node Id Is %u.\n",cur_node->node_id);

    if(NULL==pro_ele){
        err_log("PROXY : Cannot Find Current Node's Address Section.\n");
        goto goto_config_error;
    }

    const char* db_name;
    if(!config_setting_lookup_string(pro_ele,"db_name",&db_name)){
        goto goto_config_error;
    }
    size_t db_name_len = strlen(db_name);
    cur_node->db_name = (char*)malloc(sizeof(char)*(db_name_len+1));
    if(cur_node->db_name==NULL){
        goto goto_config_error;
    }
    if(NULL==strncpy(cur_node->db_name,db_name,db_name_len)){
        free(cur_node->db_name);
        goto goto_config_error;
    }
    cur_node->db_name[db_name_len] = '\0';

// parse server address

    config_setting_t *server_config;
    server_config = config_lookup(&config_file,"server_config");

    if(NULL==server_config){
        err_log("cannot find nodes settings \n");
        goto goto_config_error;
    }    

    if(NULL==server_config){
        err_log("cannot find node address section \n");
        goto goto_config_error;
    }
    config_setting_t *serv_ele = config_setting_get_elem(server_config,cur_node->node_id);
    if(NULL==serv_ele){
        err_log("cannot find current node's address\n");
        goto goto_config_error;
    }

    const char* peer_ipaddr=NULL;
    int peer_port=-1;
    if(!config_setting_lookup_string(serv_ele,"ip_address",&peer_ipaddr)){
        goto goto_config_error;
    }
    if(!config_setting_lookup_int(serv_ele,"port",&peer_port)){
        goto goto_config_error;
    }

    cur_node->sys_addr.s_addr.sin_port = htons(peer_port);
    cur_node->sys_addr.s_addr.sin_family = AF_INET;
    inet_pton(AF_INET,peer_ipaddr,&cur_node->sys_addr.s_addr.sin_addr);

    cur_node->sys_addr.s_sock_len = sizeof(cur_node->sys_addr.s_addr);


    config_destroy(&config_file);
    return 0;

goto_config_error:
    err_log("%s:%d - %s\n", config_error_file(&config_file),
            config_error_line(&config_file), config_error_text(&config_file));
    config_destroy(&config_file);
    return -1;
}