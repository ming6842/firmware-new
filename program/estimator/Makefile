export WORKSPACE_DIR=..
#
#include makefiles
include $(WORKSPACE_DIR)/makefiles/toolchain.mk
include $(WORKSPACE_DIR)/makefiles/workspace.mk
include $(WORKSPACE_DIR)/makefiles/rules.mk

LIB_ESTIMATOR=libestimator.a
ESTIMATOR_SRC = $(WORKSPACE_DIR)/estimator
ESTIMATOR_OBJS=	$(ESTIMATOR_SRC)/attitude_estimator.o \
	$(ESTIMATOR_SRC)/vertical_estimator.o \
	$(ESTIMATOR_SRC)/estimator.o
#make target
all: $(LIB_ESTIMATOR)

$(LIB_ESTIMATOR): $(ESTIMATOR_OBJS)
	$(AR) -r $(LIB_ESTIMATOR) $(ESTIMATOR_OBJS)
	$(RANLIB) $(LIB_ESTIMATOR)
clean:
	rm -rf $(LIB_ESTIMATOR)