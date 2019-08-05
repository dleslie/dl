#if defined(ENABLE_TWEEN_TEST)
#define DL_IMPLEMENTATION 1
#include "dl.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

const dl_integer width = 800;
const dl_integer height = 600;

typedef struct {
  dl_ease_direction dir;
  dl_easing_function ease;
  dl_selector_function selector;
  dl_selector_function_point2 selector_point2;
  const char *name;
} tween_method;

#define tween_count 38
#define point_count 16

dl_integer active_method = -1;
tween_method tween_methods[tween_count] = {
  {0, NULL, dl_select_linear, NULL, "dl_select_linear"},
  {0, NULL, dl_select_catmullrom, NULL, "dl_select_catmullrom"},
  {0, NULL, NULL, dl_select_linear_point2, "dl_select_linear_point2"},
  {0, NULL, NULL, dl_select_bezier_point2, "dl_select_bezier_point2"},
  {0, NULL, NULL, dl_select_catmullrom_point2, "dl_select_catmullrom_point2"},

  {DL_EASE_IN, dl_ease_quadratic, NULL, NULL, "DL_EASE_IN quadratic"},
  {DL_EASE_OUT, dl_ease_quadratic, NULL, NULL, "DL_EASE_OUT quadratic"},
  {DL_EASE_INOUT, dl_ease_quadratic, NULL, NULL, "DL_EASE_INOUT quadratic"},
  {DL_EASE_IN, dl_ease_linear, NULL, NULL, "DL_EASE_IN linear"},
  {DL_EASE_OUT, dl_ease_linear, NULL, NULL, "DL_EASE_OUT linear"},
  {DL_EASE_INOUT, dl_ease_linear, NULL, NULL, "DL_EASE_INOUT linear"},
  {DL_EASE_IN, dl_ease_cubic, NULL, NULL, "DL_EASE_IN cubic"},
  {DL_EASE_OUT, dl_ease_cubic, NULL, NULL, "DL_EASE_OUT cubic"},
  {DL_EASE_INOUT, dl_ease_cubic, NULL, NULL, "DL_EASE_INOUT cubic"},
  {DL_EASE_IN, dl_ease_quartic, NULL, NULL, "DL_EASE_IN quartic"},
  {DL_EASE_OUT, dl_ease_quartic, NULL, NULL, "DL_EASE_OUT quartic"},
  {DL_EASE_INOUT, dl_ease_quartic, NULL, NULL, "DL_EASE_INOUT quartic"},
  {DL_EASE_IN, dl_ease_quintic, NULL, NULL, "DL_EASE_IN quintic"},
  {DL_EASE_OUT, dl_ease_quintic, NULL, NULL, "DL_EASE_OUT quintic"},
  {DL_EASE_INOUT, dl_ease_quintic, NULL, NULL, "DL_EASE_INOUT quintic"},
  {DL_EASE_IN, dl_ease_sinusoidal, NULL, NULL, "DL_EASE_IN sinusoidal"},
  {DL_EASE_OUT, dl_ease_sinusoidal, NULL, NULL, "DL_EASE_OUT sinusoidal"},
  {DL_EASE_INOUT, dl_ease_sinusoidal, NULL, NULL, "DL_EASE_INOUT sinusoidal"},
  {DL_EASE_IN, dl_ease_exponential, NULL, NULL, "DL_EASE_IN exponential"},
  {DL_EASE_OUT, dl_ease_exponential, NULL, NULL, "DL_EASE_OUT exponential"},
  {DL_EASE_INOUT, dl_ease_exponential, NULL, NULL, "DL_EASE_INOUT exponential"},
  {DL_EASE_IN, dl_ease_circular, NULL, NULL, "DL_EASE_IN circular"},
  {DL_EASE_OUT, dl_ease_circular, NULL, NULL, "DL_EASE_OUT circular"},
  {DL_EASE_INOUT, dl_ease_circular, NULL, NULL, "DL_EASE_INOUT circular"},
  {DL_EASE_IN, dl_ease_elastic, NULL, NULL, "DL_EASE_IN elastic"},
  {DL_EASE_OUT, dl_ease_elastic, NULL, NULL, "DL_EASE_OUT elastic"},
  {DL_EASE_INOUT, dl_ease_elastic, NULL, NULL, "DL_EASE_INOUT elastic"},
  {DL_EASE_IN, dl_ease_back, NULL, NULL, "DL_EASE_IN back"},
  {DL_EASE_OUT, dl_ease_back, NULL, NULL, "DL_EASE_OUT back"},
  {DL_EASE_INOUT, dl_ease_back, NULL, NULL, "DL_EASE_INOUT back"},
  {DL_EASE_IN, dl_ease_bounce, NULL, NULL, "DL_EASE_IN bounce"},
  {DL_EASE_OUT, dl_ease_bounce, NULL, NULL, "DL_EASE_OUT bounce"},
  {DL_EASE_INOUT, dl_ease_bounce, NULL, NULL, "DL_EASE_INOUT bounce"},
};

dl_point2 points[point_count];
dl_real ys[point_count];

void draw(double time, double delta_time) {
  ALLEGRO_COLOR white = al_map_rgba_f(1.0, 1.0, 1.0, 1.0);
  ALLEGRO_COLOR red = al_map_rgba_f(1.0, 0.0, 0.0, 1.0);
  ALLEGRO_COLOR green = al_map_rgba_f(0.0, 1.0, 0.0, 1.0);
  tween_method current = tween_methods[active_method];
  dl_natural idx;
  dl_real next_p, p;
  dl_point2 p0, p1;
  const dl_real step = 1.0 / (dl_real)(point_count - 1) / 10.0;

  al_draw_line(0, 0.75 * height, width, 0.75 * height, green, 1.0);
  al_draw_line(0, 0.25 * height, width, 0.25 * height, green, 1.0);

  if (current.selector != NULL) {
    for (idx = 0; idx < point_count; ++idx) {
      al_draw_filled_circle(
        (float)idx / (float)(point_count - 1) * width,
        (ys[idx] * height * 0.5) + (height * 0.25),
        3.0, red);
    }
  }
  if (current.selector_point2 != NULL) {
    for (idx = 0; idx < point_count; ++idx) {
      al_draw_filled_circle(
        points[idx].x * width,
        (points[idx].y * height * 0.5) + (height * 0.25),
        3.0, red);
    }
  }

  for (p = 0; p < 1; p += step) {
    next_p = dl_clamp01(p + step);
    p0.x = p;
    p1.x = next_p;

    if (current.ease != NULL) {
      p0.y = dl_tween(current.ease, current.dir, p);
      p1.y = dl_tween(current.ease, current.dir, next_p);
    } else if (current.selector != NULL) {
      dl_interpolate(current.selector, ys, point_count, p0.x, &p0.y);
      dl_interpolate(current.selector, ys, point_count, p1.x, &p1.y);
    } else {
      dl_interpolate_point2(current.selector_point2, points, point_count, p0.x, &p0);
      dl_interpolate_point2(current.selector_point2, points, point_count, p1.x, &p1);
    }

    al_draw_line(p0.x * width, (p0.y * height * 0.5) + (height * 0.25),
                 p1.x * width, (p1.y * height * 0.5) + (height * 0.25),
                 white, 1.0);
  }
}

void change(dl_random_state *r) {
  dl_natural idx;
  dl_real last_x, last_y, t;
  dl_bool do_random, type_change;
  dl_integer last_method = active_method;

  active_method = (active_method + 1) % tween_count;
  do_random = tween_methods[active_method].selector_point2 != NULL;
  type_change = last_method < 0 || (tween_methods[active_method].selector != NULL && tween_methods[last_method].selector == NULL) || (tween_methods[active_method].selector == NULL && tween_methods[last_method].selector != NULL) || (tween_methods[active_method].selector_point2 != NULL && tween_methods[last_method].selector_point2 == NULL) || (tween_methods[active_method].selector_point2 == NULL && tween_methods[last_method].selector_point2 != NULL) || (tween_methods[active_method].ease != NULL && tween_methods[last_method].ease == NULL) || (tween_methods[active_method].ease == NULL && tween_methods[last_method].ease != NULL);
  last_x = last_y = 0.5;

  if (type_change) {
    for (idx = 0; idx < point_count; idx++) {
      t = last_x + dl_random_real_range(r, -0.15, 0.15);
      last_x = do_random
                 ? dl_clamp01(t)
                 : (float)idx / (float)(point_count - 1);

      t = last_y + dl_random_real_range(r, -0.15, 0.15);
      ys[idx] = last_y = dl_clamp01(t);
      dl_init_point2(&points[idx], last_x, last_y);
    }
  }
}

int main(int argc, char **argv) {
  ALLEGRO_DISPLAY *display;
  ALLEGRO_EVENT_QUEUE *events;
  ALLEGRO_BITMAP *bkg;
  ALLEGRO_COLOR black = al_map_rgba_f(0.0, 0.0, 0.0, 1.0);

  dl_integer screen_width = width;
  dl_integer screen_height = height;
  dl_bool should_exit = false;

  dl_random_state r;
  double time, last_tick, delta_time;

  if (!al_init()) {
    return -1;
  }

  al_init_primitives_addon();

  al_set_new_display_flags(ALLEGRO_OPENGL);
  display = al_create_display(screen_width, screen_height);
  if (!display) {
    return -1;
  }

  if (!al_install_keyboard()) {
    return -1;
  }

  if (!al_install_mouse()) {
    return -1;
  }

  bkg = al_create_bitmap(width, height);
  if (!bkg) {
    return -1;
  }

  dl_init_random_time(&r);

  change(&r);

  events = al_create_event_queue();
  al_register_event_source(events, al_get_keyboard_event_source());
  al_register_event_source(events, al_get_mouse_event_source());
  al_register_event_source(events, al_get_display_event_source(display));

  al_set_window_title(display, "Tween Test");

  last_tick = al_get_time();

  while (!should_exit) {
    ALLEGRO_EVENT event;

    while (al_get_next_event(events, &event)) {
      switch (event.type) {
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
          should_exit = true;
          break;
        case ALLEGRO_EVENT_KEY_UP:
        case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
          change(&r);
          break;
      }
    }

    al_set_target_bitmap(bkg);
    al_clear_to_color(black);

    time = al_get_time();
    delta_time = time - last_tick;
    last_tick = time;

    draw(time, delta_time);

    al_set_target_backbuffer(display);
    al_draw_scaled_bitmap(bkg,
                          0, 0, al_get_bitmap_width(bkg), al_get_bitmap_height(bkg),
                          0, 0, screen_width, screen_height,
                          0);

    al_flip_display();
  }

  return 0;
}
#endif