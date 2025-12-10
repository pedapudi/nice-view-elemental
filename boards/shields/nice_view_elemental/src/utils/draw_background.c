#include "../../include/utils/draw_background.h"

#include <lvgl.h>
#include "../../include/images/logo_0.h"
#include "../../include/images/logo_1.h"
#include "../../include/images/logo_2.h"
#include "../../include/images/logo_3.h"
#include "../../include/images/logo_4.h"
#include "../../include/images/logo_5.h"
#include "../../include/images/logo_6.h"
#include "../../include/images/logo_7.h"
#include "../../include/images/logo_8.h"
#include "../../include/images/logo_9.h"
#include "../../include/images/logo_10.h"
#include "../../include/images/logo_11.h"
#include "../../include/images/logo_12.h"
#include "../../include/images/logo_13.h"
#include "../../include/images/logo_14.h"
#include "../../include/images/logo_15.h"
#include "../../include/images/logo_16.h"
#include "../../include/images/logo_17.h"
#include "../../include/images/logo_18.h"
#include "../../include/images/logo_19.h"
#include "../../include/images/logo_20.h"
#include "../../include/images/logo_21.h"
#include "../../include/images/logo_22.h"
#include "../../include/images/logo_23.h"
#include "../../include/images/logo_24.h"

void draw_background(lv_obj_t* canvas, unsigned index) {
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);

    switch (index % 7) {
        case 0: {
            lv_canvas_draw_img(canvas, 0, 0, &logo_0, &img_dsc);
            break;
        }
        case 1: {
            lv_canvas_draw_img(canvas, 0, 0, &logo_4, &img_dsc);
            break;
        }
        case 2: {
            lv_canvas_draw_img(canvas, 0, 0, &logo_8, &img_dsc);
            break;
        }
        case 3: {
            lv_canvas_draw_img(canvas, 0, 0, &logo_12, &img_dsc);
            break;
        }
        case 4: {
            lv_canvas_draw_img(canvas, 0, 0, &logo_16, &img_dsc);
            break;
        }
        case 5: {
            lv_canvas_draw_img(canvas, 0, 0, &logo_20, &img_dsc);
            break;
        }
        case 6: {
            lv_canvas_draw_img(canvas, 0, 0, &logo_24, &img_dsc);
            break;
        }
    }
}
