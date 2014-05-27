
// #define MPU6050_I2C                  I2C2
// #define MPU6050_I2C_RCC_Periph       RCC_APB1Periph_I2C2
// #define MPU6050_I2C_Port             GPIOB
// #define MPU6050_I2C_SCL_Pin          GPIO_Pin_10
// #define MPU6050_I2C_SDA_Pin          GPIO_Pin_11
// #define MPU6050_I2C_RCC_Port         RCC_APB2Periph_GPIOB
//hardware abstraction layer
#define MPU6050_SPI_RCC_Periph SPI4
#define MPU9250_SPI_Port GPIOE
#define MPU9250_SPI_CS_PIN GPIO_Pin_4
#define MPU9250_SPI_MISO_PIN GPIO_Pin_5
#define MPU9250_SPI_MOSI_PIN GPIO_Pin_6
#define MPU9250_SPI_SCK_PIN GPIO_Pin_2
uint8_t mpu9250_read_byte(uint8_t );
void mpu9250_write_byte(uint8_t ,uint8_t );
uint8_t mpu9250_read_who_am_i();
void mpu9250_reset();

