APPLICATION ?= $(notdir $(patsubst %/,%,$(CURDIR)))

ifneq (,$(wildcard $(CURDIR)/tests/.))
  DEFAULT_MODULE += test_utils_interactive_sync
endif

ifneq (,$(filter tests_driver_%,$(APPLICATION)))
  BOARD ?= samr21-xpro
endif
BOARD ?= native
RIOTBASE ?= $(CURDIR)/../..
QUIET ?= 1
# DEVELHELP enabled by default for all tests, set 0 to disable
DEVELHELP ?= 1

# Include packages that pull up and auto-init the link layer.
# NOTE: 6LoWPAN will be included if IEEE802.15.4 devices are present
USEMODULE += gnrc_netdev_default
# Activate ICMPv6 error messages
USEMODULE += gnrc_icmpv6_error
# Specify the mandatory networking modules for IPv6 and UDP
USEMODULE += gnrc_udp
# Add a routing protocol
USEMODULE += gnrc_rpl
USEMODULE += gnrc_rpl_srh
USEMODULE += auto_init_gnrc_rpl
USEMODULE += auto_init_gnrc_netif
USEMODULE += enc28j60
USEMODULE += gnrc_ipv6_router_default
USEMODULE += gnrc_icmpv6_echo
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps
USEMODULE += netstats_l2
USEMODULE += netstats_ipv6
USEMODULE += netstats_rpl
USEMODULE += gnrc_pktdump

# Configure 2:nd address
CFLAGS += -DCONFIG_GNRC_IPV6_NIB_SLAAC=1

# Specify the minimum networking modules for IPv6
#USEMODULE += gnrc_ipv6
# Additional networking modules that can be dropped if not needed
# Use minimal standard PRNG
#USEMODULE += prng_minstd

# set board specific peripheral configurations
ifneq (,$(filter stm32f4discovery,$(BOARD)))
  ENC_SPI ?= SPI_DEV\(1\)
  ENC_CS  ?= GPIO_PIN\(PORT_B,12\)
  ENC_INT ?= GPIO_PIN\(PORT_B,11\)
  ENC_RST ?= GPIO_PIN\(PORT_B,10\)
endif

ifneq (,$(filter avr-rss2,$(BOARD)))
ENC_CS = GPIO_PIN\(PORT_D,6\)
ENC_INT = GPIO_PIN\(PORT_D,2\)
ENC_RST = GPIO_PIN\(PORT_D,7\)
endif

# fallback: set SPI bus and pins to default values
ENC_SPI ?= SPI_DEV\(0\)
ENC_CS  ?= GPIO_PIN\(0,0\)
ENC_INT ?= GPIO_PIN\(0,1\)
ENC_RST ?= GPIO_PIN\(0,2\)
# export SPI and pins
CFLAGS += -DENC28J60_PARAM_SPI=$(ENC_SPI)
CFLAGS += -DENC28J60_PARAM_CS=$(ENC_CS)
CFLAGS += -DENC28J60_PARAM_INT=$(ENC_INT)
CFLAGS += -DENC28J60_PARAM_RESET=$(ENC_RST)

# make sure we read the local enc28j60 params file
CFLAGS += -I$(CURDIR)

include $(RIOTBASE)/Makefile.include

# Set a custom channel 
# For KTH Contiki gateway
#
DEFAULT_CHANNEL=25
DEFAULT_PAN_ID=0xFEED
include $(RIOTMAKE)/default-radio-settings.inc.mk
