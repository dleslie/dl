#define DL_IMPLEMENTATION 1
#include "dl.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

const integer width = 640;
const integer height = 480;

typedef struct {
  ease_direction dir;
  easing_function ease;
  selector_function selector;
  selector_function_point2 selector_point2;
  const char *name;
} tween_method;

#define tween_count 39
#define point_count 16

integer active_method = -1;
tween_method tween_methods[tween_count] = {
  { 0, NULL, select_linear, NULL, "select_linear" },
  { 0, NULL, select_bezier, NULL, "select_bezier" },
  { 0, NULL, select_catmullrom, NULL, "select_catmullrom" },

  { 0, NULL, NULL, select_linear_point2, "select_linear_point2" },
  { 0, NULL, NULL, select_bezier_point2, "select_bezier_point2" },
  { 0, NULL, NULL, select_catmullrom_point2, "select_catmullrom_point2" },

  { EASE_IN, ease_linear, NULL, NULL, "EASE_IN linear" },
  { EASE_OUT, ease_linear, NULL, NULL, "EASE_OUT linear" },
  { EASE_INOUT, ease_linear, NULL, NULL, "EASE_INOUT linear" },
  { EASE_IN, ease_quadratic, NULL, NULL, "EASE_IN quadratic" },
  { EASE_OUT, ease_quadratic, NULL, NULL, "EASE_OUT quadratic" },
  { EASE_INOUT, ease_quadratic, NULL, NULL, "EASE_INOUT quadratic" },
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

vec2 points[point_count];

void draw(double time, double delta_time) {
  ALLEGRO_COLOR white = al_map_rgba_f(1.0, 1.0, 1.0, 1.0);
  ALLEGRO_COLOR red = al_map_rgba_f(1.0, 0.0, 0.0, 1.0);
  ALLEGRO_COLOR green = al_map_rgba_f(0.0, 1.0, 0.0, 1.0);
  tween_method current = tween_methods[active_method];

  al_draw_line(0, 0.75 * height, width, 0.75 * height, green, 1.0);
  al_draw_line(0, 0.25 * height, width, 0.25 * height, green, 1.0);

  if (current.selector != NULL) {
    for (natural idx = 0; idx < point_count; ++idx) {
      al_draw_filled_circle(((real)width / (real)(point_count - 1)) * (real)(idx),
			    (points[idx].y * height * 0.5) + (height * 0.25),
			    3.0, red);
    }
  }
  if (current.selector_point2 != NULL) {
    for (natural idx = 0; idx < point_count; ++idx) {
      al_draw_filled_circle(points[idx].x * width,
			    (points[idx].y * height * 0.5) + (height * 0.25),
			    3.0, red);
    }
  }

  for (real p = 0; p < 1; p += 0.001) {
    real last_p = clamp01(p - 0.001);
    point2 p0, p1;
    
    if (current.ease != NULL) {
      p0.x = last_p;
      p1.x = p;
      p0.y = tween(current.ease, current.dir, last_p);
      p1.y = tween(current.ease, current.dir, p);
    } else if (current.selector != NULL) {
      real ys[point_count];
      for (natural pidx = 0; pidx < point_count; ++pidx)
	ys[pidx] = points[pidx].y;
      
      p0.x = last_p;
      p1.x = p;
      interpolate(current.selector, ys, point_count, last_p, &p0.y);
      interpolate(current.selector, ys, point_count, p, &p1.y); 
    }
    else {
      interpolate_point2(current.selector_point2, points, point_count, last_p, &p0);
      interpolate_point2(current.selector_point2, points, point_count, p, &p1);
    }
    
    al_draw_line(p0.x * width, (p0.y * height * 0.5) + (height * 0.25),
		 p1.x * width, (p1.y * height * 0.5) + (height * 0.25),
		 white, 1.0);
  }
}

void change(random_state *r) {
  bool was_interpolate = active_method >= 0 && (tween_methods[active_method].selector != NULL || tween_methods[active_method].selector_point2 != NULL);
  active_method = (active_method + 1) % tween_count;
  
  INFO("%s", tween_methods[active_method].name);

  if (!was_interpolate) {
    for (natural idx = 0; idx < point_count; idx++) {
      real x = idx == 0 ? 0 : (idx == point_count - 1 ? 1 : random_real_range(r, 0, 1.0/(float)point_count) + (float)idx/(float)point_count);
      init_vec2(&points[idx], x, random_real_range(r, 0, 1));
    }
  }
}

int main(int argc, char **argv)
{
  ALLEGRO_DISPLAY *display;
  ALLEGRO_EVENT_QUEUE *events;
  ALLEGRO_BITMAP *bkg;
  ALLEGRO_COLOR black = al_map_rgba_f(0.0, 0.0, 0.0, 1.0);
  
  integer screen_width = width;
  integer screen_height = height;
  
  if (!al_init()) {
    ERROR("Could not initialize Allegro.");
    return -1;
  }

  al_init_primitives_addon();

  al_set_new_display_flags(ALLEGRO_OPENGL);
  display = al_create_display(screen_width, screen_height);
  if (!display) {
    ERROR("Could not create display.");
    return -1;
  }

  if (!al_install_keyboard()) {
    ERROR("Could not load keyboard.");
    return -1;
  }

  if (!al_install_mouse()) {
    ERROR("Could not load mouse.");
    return -1;
  }

  bkg = al_create_bitmap(width, height);
  if (!bkg) {
    ERROR("Could not create background image.");
    return -1;
  }

  random_state r;
  init_random_time(&r);
  
  change(&r);
  
  events = al_create_event_queue();
  al_register_event_source(events, al_get_keyboard_event_source());
  al_register_event_source(events, al_get_mouse_event_source());
  al_register_event_source(events, al_get_display_event_source(display));

  al_set_window_title(display, "Tween Test");

  double last_tick = al_get_time();
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
    
    double time = al_get_time();
    double delta_time = time - last_tick;
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
