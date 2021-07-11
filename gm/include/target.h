#ifndef __TARGET_H_
#define __TARGET_H_

#define LOWER(x) ((x) | 32)

extern char *optarg;

enum target_error { BAD_GHOSTID = 1, LARGE_GHOSTID };

enum target_error set_target(void);

#endif /* __TARGET_H_ */
