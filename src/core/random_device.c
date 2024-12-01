#include <stdio.h>
#include <stdlib.h>

#include "random.h"
#include "random_device.h"

#ifndef _WIN32

#include <stddef.h>
#include <stdbool.h>

#include <assert.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>

struct RandomDevice {
  int fd;
};

size_t random_device_read(random_device_t *instance, void *buffer,
                          size_t count) {
  return read(instance->fd, buffer, count);
}

void random_device_dtor(random_device_t *instance) {
  close(instance->fd);
  free(instance);
}

static bool streq(const char * s1, const char * s2) {
  return strcmp(s1, s2) == 0;
}

static const char * SUPPORTED_TOKEN[] = {
  "/dev/urandom",
  "/dev/random",
};

random_device_t *random_device_ctor_token(const char *token) {
  bool is_default = token == NULL || token[0] == '\0' || streq(token, "default");
  if (is_default) {
    token = "/dev/urandom";
  } else {
    bool supported = false;
    for (size_t i = 0; i < sizeof(SUPPORTED_TOKEN) / sizeof(SUPPORTED_TOKEN[0]); i++) {
        if (streq(token, SUPPORTED_TOKEN[i])) {
            supported = true;
        }
    }
    if (!supported) {
        perror("random_device_ctor_token: unsupported token");
        exit(1);
        return NULL;
    }
  }
  int fd = open(token, O_RDONLY);
  if (fd == -1) {
    return NULL;
  }
  random_device_t *rd = (random_device_t *)malloc(sizeof(random_device_t));
  rd->fd = fd;
  return rd;
}

random_device_t *random_device_ctor(void) {
  return random_device_ctor_token(NULL);
}

#endif /* !defined(_WIN32) */

#ifdef _WIN32

#include <stdint.h>

#include <windows.h>

struct RandomDevice {
    HCRYPTPROV hCryptProv;
};

random_device_t *random_device_ctor(void) {
    random_device_t *rd = (random_device_t *)malloc(sizeof(random_device_t));
    if (!CryptAcquireContext(&rd->hCryptProv, NULL, NULL, PROV_RSA_FULL,
                             CRYPT_VERIFYCONTEXT)) {
      fprintf(stderr, "Error acquiring crypto context: %ld\n", GetLastError());
      exit(1);
    }
    return rd;
}

random_device_t *random_device_ctor_token(const char *token) {
    (void) token; // unused
    return random_device_ctor();
}

size_t random_device_read(random_device_t *instance, void *buffer,
                          size_t count) {
  if (!CryptGenRandom(instance->hCryptProv, count, buffer)) {
    return -1;
  }
  return count;
}

void random_device_dtor(random_device_t *instance) {
  CryptReleaseContext(instance->hCryptProv, 0);
  free(instance);
}

#endif /* _WIN32 */

size_t random_device_read_s(random_device_t *rd, void *buffer,
                            size_t buffer_size, size_t count) {
  return random_device_read(rd, buffer,
                            count < buffer_size ? count : buffer_size);
}

random_engine_t * random_device_engine_ctor(void) {
  random_device_t * data = random_device_ctor();
  if (data == NULL) {
    return NULL;
  }
  random_engine_t * engine = random_engine_ctor((random_engine_spec_t) &RandomDeviceEngine, data);
  return engine;
}

void random_device_engine_dtor(random_engine_t *engine) {
  random_device_t * rd = random_engine_data(engine);
  random_device_dtor(rd);
}

uint64_t random_device_engine_next(random_engine_t *engine) {
  uint64_t result;
  random_device_t *instance = random_engine_data(engine);
  random_device_read_s(instance, &result, sizeof(result), sizeof(result));
  return result;
}

const struct RandomEngineSpec RandomDeviceEngine = {
    .name = "RandomDevice",
    .ctor = random_device_engine_ctor,
    .dtor = random_device_engine_dtor,
    .next = random_device_engine_next,
};
