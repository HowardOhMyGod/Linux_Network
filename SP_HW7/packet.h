#include <sys/types.h>
#include <sys/ipc.h>


#define ID_MAX 5000

#define SHM_NAME "/shm"
#define IDX_SHARE "/shm_idx"

typedef struct packet {
  int id;
  short dataShort[5];
  long dataLong[5];
  double dataDouble[5];
  char dataByte[6];
} Packet;

#define SHM_KEY ftok("/tmp", 10)
#define SHM_KEY_IDX ftok("/tmp2", 10)
