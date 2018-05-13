
# Author, Copyright: Oleg Borodin <onborodin@gmail.com> 2018

.SECONDARY:

SPL_LOC=     SPL
CMSIS_LOC=   CMSIS
RTOS_LOC=    RTOS


CFLAGS+= -I. -Os -mthumb -mcpu=cortex-m3 -march=armv7-m -msoft-float -DSTM32F1 -std=c99
CFLAGS+= -fno-common -ffunction-sections -fdata-sections
CFLAGS+= -g -gdwarf-2
CFLAGS+= -I$(CMSIS_LOC) -I$(SPL_LOC) -I$(RTOS_LOC)

LDFLAGS+= ${CFLAGS}
LDFLAGS+= --static
LDFLAGS+= -nostartfiles
LDFLAGS+= -T master.ld

LDFLAGS+= -Wl,-Map=master.map
LDFLAGS+= -Wl,--cref -Wl,--gc-sections
LDFLAGS+= -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group
LDFLAGS+= -L$(RTOS_LOC) -lrtos
LDFLAGS+= -L$(SPL_LOC) -lstm32f10x
LDFLAGS+= -L$(CMSIS_LOC) -lcmsis
LDFLAGS+= -lopencm3_stm32f1

CFLAGS+= -DSTM32F10X_MD=1 -DUSE_STDPERIPH_DRIVER -DSYSCLK_FREQ_72MHz=1

SPL_OBJS+= $(SPL_LOC)/stm32f10x_adc.o
SPL_OBJS+= $(SPL_LOC)/stm32f10x_bkp.o
SPL_OBJS+= $(SPL_LOC)/stm32f10x_can.o
SPL_OBJS+= $(SPL_LOC)/stm32f10x_cec.o
SPL_OBJS+= $(SPL_LOC)/stm32f10x_crc.o
SPL_OBJS+= $(SPL_LOC)/stm32f10x_dac.o
SPL_OBJS+= $(SPL_LOC)/stm32f10x_dbgmcu.o
SPL_OBJS+= $(SPL_LOC)/stm32f10x_dma.o
SPL_OBJS+= $(SPL_LOC)/stm32f10x_exti.o
SPL_OBJS+= $(SPL_LOC)/stm32f10x_flash.o
SPL_OBJS+= $(SPL_LOC)/stm32f10x_fsmc.o
SPL_OBJS+= $(SPL_LOC)/stm32f10x_gpio.o
SPL_OBJS+= $(SPL_LOC)/stm32f10x_i2c.o
SPL_OBJS+= $(SPL_LOC)/stm32f10x_iwdg.o
SPL_OBJS+= $(SPL_LOC)/stm32f10x_pwr.o
SPL_OBJS+= $(SPL_LOC)/stm32f10x_rcc.o
SPL_OBJS+= $(SPL_LOC)/stm32f10x_rtc.o
SPL_OBJS+= $(SPL_LOC)/stm32f10x_sdio.o
SPL_OBJS+= $(SPL_LOC)/stm32f10x_spi.o
SPL_OBJS+= $(SPL_LOC)/stm32f10x_tim.o
SPL_OBJS+= $(SPL_LOC)/stm32f10x_usart.o
SPL_OBJS+= $(SPL_LOC)/stm32f10x_wwdg.o
SPL_OBJS+= $(SPL_LOC)/misc.o

CMSIS_OBJS+= $(CMSIS_LOC)/system_stm32f10x.o

RTOS_OBJS+= $(RTOS_LOC)/croutine.o
RTOS_OBJS+= $(RTOS_LOC)/event_groups.o
RTOS_OBJS+= $(RTOS_LOC)/heap_4.o
RTOS_OBJS+= $(RTOS_LOC)/list.o
RTOS_OBJS+= $(RTOS_LOC)/port.o
RTOS_OBJS+= $(RTOS_LOC)/queue.o
RTOS_OBJS+= $(RTOS_LOC)/stream_buffer.o
RTOS_OBJS+= $(RTOS_LOC)/tasks.o
RTOS_OBJS+= $(RTOS_LOC)/timers.o




MASTER_OBJS+= master.o
MASTER_OBJS+= startup.o
MASTER_OBJS+= syscall.o

all: rtos spl cmsis master.bin


rtos:  $(RTOS_LOC)/librtos.a

$(RTOS_LOC)/librtos.a: $(RTOS_OBJS)
	cd $(@D) && arm-eabi-ar rcv $(@F) $(^F)

cmsis:  $(CMSIS_LOC)/libcmsis.a

$(CMSIS_LOC)/libcmsis.a: $(CMSIS_OBJS)
	cd $(@D) && arm-eabi-ar rcv $(@F) $(^F)

spl:  $(SPL_LOC)/libstm32f10x.a

$(SPL_LOC)/libstm32f10x.a: $(SPL_OBJS)
	cd $(@D) && arm-eabi-ar rcv $(@F) $(^F)


master.elf: $(MASTER_OBJS)
	arm-eabi-gcc $(^F) $(LDFLAGS) -o $@ 
	arm-eabi-size --format=berkeley $@

%.o: %.c
	arm-eabi-gcc $(CFLAGS) -c -o $@ $<

%.o: %.S
	arm-eabi-as $(ASFLAGS) -o $@ $<

%.bin: %.elf
	arm-eabi-objcopy -O binary $< $@

%.elf: %.o
	arm-eabi-gcc $(^F) $(LDFLAGS) -o $@ 
	arm-eabi-size --format=berkeley $@

clean:
	rm -f *.i *.o *.elf *.bin *.map *~ *.hex *.d *.s
	cd $(RTOS_LOC) && rm -f *.o lib*.a

fullclean: clean
	cd $(SPL_LOC) && rm -f *.o lib*.a
	cd $(CMSIS_LOC) && rm -f *.o lib*.a


upload: all master.upl

%.upl: %.bin
	openocd \
	    -c 'puts "--- START --------------------"' \
	    -f 'interface/stlink-v2.cfg' \
	    -f 'target/stm32f1x.cfg'  \
	    -c 'puts "--- INIT --------------------"' \
	    -c "init" \
	    -c "reset halt" \
	    -c 'puts "--- WRITE --------------------"' \
	    -c "flash write_image erase $< 0x08000000"\
	    -c 'puts "--- VERIFY --------------------"' \
	    -c "verify_image $<" \
	    -c 'puts "--- RESET --------------------"' \
	    -c "reset" \
	    -c 'puts "--- DONE --------------------"' \
	    -c "shutdown"
#EOF
