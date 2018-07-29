#define DL_IMPLEMENTATION 1
#include "dl.h"

#if DL_USE_TWEEN

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

const integer width = 800;
const integer height = 600;

typedef struct {
  ease_direction dir;
  easing_function ease;
  selector_function selector;
  selector_function_point2 selector_point2;
  const char *name;
} tween_method;

#define tween_count 38
#define point_count 16

integer active_method = -1;
tween_method tween_methods[tween_count] = {
  { 0, NULL, select_linear, NULL, "select_linear" },
  { 0, NULL, select_catmullrom, NULL, "select_catmullrom" },
  { 0, NULL, NULL, select_linear_point2, "select_linear_point2" },
  { 0, NULL, NULL, select_bezier_point2, "select_bezier_point2" },
  { 0, NULL, NULL, select_catmullrom_point2, "select_catmullrom_point2" },

  { EASE_IN, ease_quadratic, NULL, NULL, "EASE_IN quadratic" },
  { EASE_OUT, ease_quadratic, NULL, NULL, "EASE_OUT quadratic" },
  { EASE_INOUT, ease_quadratic, NULL, NULL, "EASE_INOUT quadratic" },
  { EASE_IN, ease_linear, NULL, NULL, "EASE_IN linear" },
  { EASE_OUT, ease_linear, NULL, NULL, "EASE_OUT linear" },
  { EASE_INOUT, ease_linear, NULL, NULL, "EASE_INOUT linear" },
  { EASE_IN, ease_cubic, NULL, NULL, "EASE_IN cubic" },
  { EASE_OUT, ease_cubic, NULL, NULL, "EASE_OUT cubic" },
  { EASE_INOUT, ease_cubic, NULL, NULL, "EASE_INOUT cubic" },
  { EASE_IN, ease_quartic, NULL, NULL, "EASE_IN quartic" },
  { EASE_OUT, ease_quartic, NULL, NULL, "EASE_OUT quartic" },
  { EASE_INOUT, ease_quartic, NULL, NULL, "EASE_INOUT quartic" },
  { EASE_IN, ease_quintic, NULL, NULL, "EASE_IN quintic" },
  { EASE_OUT, ease_quintic, NULL, NULL, "EASE_OUT quintic" },
  { EASE_INOUT, ease_quintic, NULL, NULL, "EASE_INOUT quintic" },
  { EASE_IN, ease_sinusoidal, NULL, NULL, "EASE_IN sinusoidal" },
  { EASE_OUT, ease_sinusoidal, NULL, NULL, "EASE_OUT sinusoidal" },
  { EASE_INOUT, ease_sinusoidal, NULL, NULL, "EASE_INOUT sinusoidal" },
  { EASE_IN, ease_exponential, NULL, NULL, "EASE_IN exponential" },
  { EASE_OUT, ease_exponential, NULL, NULL, "EASE_OUT exponential" },
  { EASE_INOUT, ease_exponential, NULL, NULL, "EASE_INOUT exponential" },
  { EASE_IN, ease_circular, NULL, NULL, "EASE_IN circular" },
  { EASE_OUT, ease_circular, NULL, NULL, "EASE_OUT circular" },
  { EASE_INOUT, ease_circular, NULL, NULL, "EASE_INOUT circular" },
  { EASE_IN, ease_elastic, NULL, NULL, "EASE_IN elastic" },
  { EASE_OUT, ease_elastic, NULL, NULL, "EASE_OUT elastic" },
  { EASE_INOUT, ease_elastic, NULL, NULL, "EASE_INOUT elastic" },
  { EASE_IN, ease_back, NULL, NULL, "EASE_IN back" },
  { EASE_OUT, ease_back, NULL, NULL, "EASE_OUT back" },
  { EASE_INOUT, ease_back, NULL, NULL, "EASE_INOUT back" },
  { EASE_IN, ease_bounce, NULL, NULL, "EASE_IN bounce" },
  { EASE_OUT, ease_bounce, NULL, NULL, "EASE_OUT bounce" },
  { EASE_INOUT, ease_bounce, NULL, NULL, "EASE_INOUT bounce" },
};

point2 points[point_count];
real ys[point_count];

void draw(double time, double delta_time) {
  ALLEGRO_COLOR white = al_map_rgba_f(1.0, 1.0, 1.0, 1.0);
  ALLEGRO_COLOR red = al_map_rgba_f(1.0, 0.0, 0.0, 1.0);
  ALLEGRO_COLOR green = al_map_rgba_f(0.0, 1.0, 0.0, 1.0);
  tween_method current = tween_methods[active_method];
  natural idx;
  real next_p, p;
  point2 p0, p1;
  const real step = 1.0 / (real)(point_count - 1) / 10.0;

  al_draw_line(0, 0.75 * height, width, 0.75 * height, green, 1.0);
  al_draw_line(0, 0.25 * height, width, 0.25 * height, green, 1.0);

  if (current.selector != NULL) {
    for (idx = 0; idx < point_count; ++idx) {
      al_draw_filled_circle(
        (float)idx/(float)(point_count - 1) * width,
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
    next_p = clamp01(p + step);
    p0.x = p;
    p1.x = next_p;
    
    if (current.ease != NULL) {
      p0.y = tween(current.ease, current.dir, p);
      p1.y = tween(current.ease, current.dir, next_p);
    }
    else if (current.selector != NULL) {
      interpolate(current.selector, ys, point_count, p0.x, &p0.y);
      interpolate(current.selector, ys, point_count, p1.x, &p1.y); 
    }
    else {
      interpolate_point2(current.selector_point2, points, point_count, p0.x, &p0);
      interpolate_point2(current.selector_point2, points, point_count, p1.x, &p1);
    }
    
    al_draw_line(p0.x * width, (p0.y * height * 0.5) + (height * 0.25),
		 p1.x * width, (p1.y * height * 0.5) + (height * 0.25),
		 white, 1.0);
  }
}

void change(random_state *r) {
  natural idx;
  real last_x, last_y, t;
  bool do_random, type_change;
  integer last_method = active_method;
  
  active_method = (active_method + 1) % tween_count;
  do_random = tween_methods[active_method].selector_point2 != NULL;
  type_change = last_method < 0 
    || (tween_methods[active_method].selector != NULL && tween_methods[last_method].selector == NULL)
    || (tween_methods[active_method].selector == NULL && tween_methods[last_method].selector != NULL)
    || (tween_methods[active_method].selector_point2 != NULL && tween_methods[last_method].selector_point2 == NULL)
    || (tween_methods[active_method].selector_point2 == NULL && tween_methods[last_method].selector_point2 != NULL)
    || (tween_methods[active_method].ease != NULL && tween_methods[last_method].ease == NULL)
    || (tween_methods[active_method].ease == NULL && tween_methods[last_method].ease != NULL);
  last_x = last_y = 0.5;
  
  if (type_change) {
    for (idx = 0; idx < point_count; idx++) {
      t = last_x + random_real_range(r, -0.15, 0.15);
      last_x = do_random
        ? clamp01(t)
        : (float)idx/(float)(point_count - 1);

      t = last_y + random_real_range(r, -0.15, 0.15);
      ys[idx] = last_y = clamp01(t);
      init_point2(&points[idx], last_x, last_y);
    }
  }
}

#endif

int main(int argc, char **argv)
{
# if DL_USE_TWEEN
  ALLEGRO_DISPLAY *display;
  ALLEGRO_EVENT_QUEUE *events;
  ALLEGRO_BITMAP *bkg;
  ALLEGRO_COLOR black = al_map_rgba_f(0.0, 0.0, 0.0, 1.0);
  
  integer screen_width = width;
  integer screen_height = height;

  random_state r;
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

  init_random_time(&r);
  
  change(&r);
  
  events = al_create_event_queue();
  al_register_event_source(events, al_get_keyboard_event_source());
  al_register_event_source(events, al_get_mouse_event_source());
  al_register_event_source(events, al_get_display_event_source(display));

  al_set_window_title(display, "Tween Test");

  last_tick = al_get_time();
  bool should_exit = false;

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

# endif
  
  return 0;
}
