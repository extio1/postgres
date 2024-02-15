MODULE_big = test_module
OBJS = src/main.o src/test.o

EXTENSION = test_module

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
