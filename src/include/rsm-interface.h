#ifndef RSM_INTERFACE_H
#define RSM_INTERFACE_H
#include <unistd.h>
#include <stdint.h>

struct proxy_node_t;

#ifdef __cplusplus
extern "C" {
#endif

	struct proxy_node_t* proxy_init(uint8_t node_id, const char* config_path);
	void proxy_on_read(struct proxy_node_t* proxy, void* buf, ssize_t ret, int fd);
	void proxy_on_accept(struct proxy_node_t* proxy, int ret);
	void proxy_on_close(struct proxy_node_t* proxy, int fildes);
	
#ifdef __cplusplus
}
#endif

#endif
