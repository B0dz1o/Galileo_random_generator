%rename("%(strip:[MRAA_])s", %$isenumitem) "";

%include stdint.i
%include std_string.i
%include exception.i

#ifdef DOXYGEN
    %include common_hpp_doc.i
    %include gpio_class_doc.i
    %include i2c_class_doc.i
    %include pwm_class_doc.i
    %include aio_class_doc.i
    %include spi_class_doc.i
    %include uart_class_doc.i
#endif

%{
    #include "common.hpp"
    #include "gpio.hpp"
    #include "pwm.hpp"
    #include "i2c.hpp"
    #include "spi.hpp"
    #include "aio.hpp"
    #include "uart.hpp"
%}

%init %{
    //Adding mraa_init() to the module initialisation process
    mraa_init();
%}

%exception {
    try {
        $action
    } catch(const std::invalid_argument& e) {
        SWIG_exception(SWIG_ValueError, e.what());
    } catch(...) {
        SWIG_exception(SWIG_RuntimeError, "Unknown exception");
    }
}

%include "common.hpp"

%include "types.h"

%include "gpio.hpp"

%include "i2c.hpp"

%include "pwm.hpp"

%include "spi.hpp"

%include "aio.hpp"

%include "uart.hpp"
