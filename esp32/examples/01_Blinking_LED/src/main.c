#include "driver/gpio.h"
#include "driver/gptimer.h"
#include "esp_err.h"

// CALLBACK FUNCTION FOR TIMER INTERRUPT
static bool alarm_cb(gptimer_handle_t timer,
                     const gptimer_alarm_event_data_t *edata, void *user_ctx)

{
  static bool LED_STATE;
  LED_STATE = !LED_STATE;
  gpio_set_level(25, LED_STATE);

  return false;
}

void init() {
  // GPIO Config
  gpio_config_t io_conf = {};
  io_conf.intr_type = GPIO_INTR_DISABLE;        // Disable interrupts
  io_conf.mode = GPIO_MODE_OUTPUT;              // Set output
  io_conf.pin_bit_mask = (1 << 25);             // Set pin 25 bitmask
  io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE; // Disable pulldown
  io_conf.pull_up_en = GPIO_PULLUP_DISABLE;     // Disable pullup
  gpio_config(&io_conf);                        // Apply GPIO config

  // Timer config
  gptimer_handle_t gptimer = NULL;
  gptimer_config_t timer_config = {
      .clk_src = GPTIMER_CLK_SRC_DEFAULT, // Select the default clock source
      .direction = GPTIMER_COUNT_UP,      // Counting direction is up
      .resolution_hz =
          1 * 1000 *
          1000, // Resolution is 1 MHz, i.e., 1 tick equals 1 microsecond
  };
  gptimer_alarm_config_t alarm_config = {

      .reload_count = 0, // When the alarm event occurs, the timer will
                         // automatically reload to 0

      .alarm_count = 1000000, // Set the actual alarm period, since the
                              // resolution is 1us, 1000000 represents 1s

      .flags.auto_reload_on_alarm = true, // Enable auto-reload function

  };

  gptimer_event_callbacks_t cbs = {
      .on_alarm = alarm_cb, // Call the user callback function when the alarm
                            // event occurs
  };

  ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &gptimer));
  ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer, &alarm_config));
  // Register timer event callback functions, allowing user context to be
  // carried
  ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &cbs, NULL));
  // Enable the timer
  ESP_ERROR_CHECK(gptimer_enable(gptimer));
  // Start the timer
  ESP_ERROR_CHECK(gptimer_start(gptimer));
}

void app_main() {
  init();
  while (1)
    ;
}
