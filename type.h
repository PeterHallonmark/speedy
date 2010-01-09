typedef struct {
    void (*init)(void);
    void (*shutdown)(void);
} daemon_t;
