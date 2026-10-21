#include <stdint.h>
void diag_before(uint32_t pc,uint32_t kind);
void diag_control(uint32_t event,uint32_t argument);
void diag_save_hidden(void);
