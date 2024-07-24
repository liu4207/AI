idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    REQUIRES spi xl9555 i2c i2s es8388 wavplay audioplay driver)
