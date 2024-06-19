# ASIC
This module controls a single chain of ASICs. The intention is that if an array of chains of ASICs are needed then the chip selects for the chains will be controlled externally.
The module uses the cmsis SPI driver to communicate to the ASICs.

## Semaphore/Flag mechanism
The struct that is passed into the initialisation function contains the functions `lockSem` and `unlockSem`. If these are left as `NULL` the module will adopt a non RTOS system of flags. This means that the module will block for writes and reads. 

### lockSem - 
This function should test a flag/semaphore and block if the flag/semaphore is locked. Once it unlocks, lock the flag/semaphore.

### unlockSem -
This function should unlock a flag/semaphore.

### Example
``` C
static void asic_callback(uint32_t event) {
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  switch (event) {
    case ARM_SPI_EVENT_TRANSFER_COMPLETE:
      clearCS(); // Needed for multiple chained systems.

      // For FreeRTOS we need to use the ISR version.
      // Otherwise unlock() would be fine.
      xSemaphoreGiveFromISR(sem, &xHigherPriorityTaskWoken); 
      break;
      ...

static void lock(void) { xSemaphoreTake(sem, portMAX_DELAY); }
static void unlock(void) { xSemaphoreGive(sem); }

asic_spi_struct asicSPI = {.spi = &Driver_SPI1,
                           .lockSem = lock,
                           .unlockSem = unlock,
                           .setCS = setCS,
                           .clearCS = clearCS};
```