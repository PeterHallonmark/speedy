typedef struct {
    void (*init)(void);
    void (*shutdown)(void);
} service_t;
