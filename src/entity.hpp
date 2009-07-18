#ifndef ENTITY_HPP_INCLUDED
#define ENTITY_HPP_INCLUDED

#include <string>

#include "boost/intrusive_ptr.hpp"

#include "current_generator.hpp"
#include "editor_variable_info.hpp"
#include "formula_callable.hpp"
#include "formula_fwd.hpp"
#include "geometry.hpp"
#include "key.hpp"
#include "wml_node_fwd.hpp"

class character;
class entity;
class frame;
class level;
class pc_character;

typedef boost::intrusive_ptr<entity> entity_ptr;
typedef boost::intrusive_ptr<const entity> const_entity_ptr;
typedef boost::intrusive_ptr<character> character_ptr;

class entity : public game_logic::formula_callable
{
public:
	static entity_ptr build(wml::const_node_ptr node);
	explicit entity(wml::const_node_ptr node);
	entity(int x, int y, bool face_right);
	virtual ~entity() {}
	virtual wml::node_ptr write() const = 0;
	virtual void setup_drawing() const {}
	virtual void draw() const = 0;
	virtual void draw_group() const = 0;
	virtual const pc_character* is_human() const { return NULL; }
	virtual pc_character* is_human() { return NULL; }
	virtual bool on_players_side() const { return false; }
	virtual void process(level& lvl) = 0;
	virtual void execute_command(const variant& var) = 0;

	const std::string& label() const { return label_; }
	void set_label(const std::string& lb) { label_ = lb; }
	void set_distinct_label();
	
	void set_pos(const point& p) { x_ = p.x*100; y_ = p.y*100; }
	void set_pos(int x, int y) { x_ = x*100; y_ = y*100; }
	void set_x(int x) { x_ = x*100; }
	void set_y(int y) { y_ = y*100; }

	int x() const { return x_/100; }
	int y() const { return y_/100; }
	virtual int zorder() const { return 0; }

	virtual int velocity_x() const { return 0; }
	virtual int velocity_y() const { return 0; }

	int group() const { return group_; }
	void set_group(int group) { group_ = group; }

	virtual bool is_standable(int x, int y, int* friction=NULL, int* traction=NULL, int* adjust_y=NULL) const { return false; }

	virtual void stood_on_by(const entity_ptr& ch) {}

	virtual bool destroyed() const = 0;

	virtual bool point_collides(int x, int y) const = 0;
	rect body_rect() const;
	rect hit_rect() const;

	virtual void hit_player() {}
	virtual void hit_by(entity& e) {}

	virtual const frame& portrait_frame() const = 0;
	virtual const frame& icon_frame() const = 0;
	virtual const frame& current_frame() const = 0;

	bool is_alpha(int xpos, int ypos) const;

	int feet_x() const;
	int feet_y() const;

	bool face_right() const { return face_right_; }
	virtual void set_face_right(bool facing);

	bool upside_down() const { return upside_down_; }
	virtual void set_upside_down(bool facing);

	int face_dir() const { return face_right() ? 1 : -1; }

	virtual bool body_harmful() const { return true; }

	virtual int time_in_frame() const = 0;

	virtual int springiness() const = 0;
	virtual bool spring_off_head(const entity& landed_on_by) { return true; }

	virtual void activation_distance(int* x, int* y);
	virtual bool dies_on_inactive() const { return false; } 
	virtual bool always_active() const { return false; } 
	
	virtual formula_callable* vars() { return NULL; }
	virtual const formula_callable* vars() const { return NULL; }

	virtual bool body_passthrough() const { return false; }

	virtual bool look_up() const { return false; }
	virtual bool look_down() const { return false; }

	void set_id(int id) { id_ = id; }
	int get_id() const { return id_; }

	bool respawn() const { return respawn_; }

	virtual bool boardable_vehicle() const { return false; }
	virtual void boarded(level& lvl, const character_ptr& player) {}

	virtual int weight() const { return 1; }
	
	virtual int mass() const = 0;

	virtual void get_powerup(const std::string& id) {}

	void draw_debug_rects() const;

	const_editor_entity_info_ptr editor_info() const { return editor_info_; }

	virtual entity_ptr clone() const { return entity_ptr(); }
	virtual entity_ptr backup() const = 0;

	virtual void generate_current(const entity& target, int* velocity_x, int* velocity_y) const;

	virtual game_logic::const_formula_ptr get_event_handler(const std::string& key) const { return game_logic::const_formula_ptr(); }
	virtual void set_event_handler(const std::string& key, game_logic::const_formula_ptr f) { return; }

	//function which returns true if this object can be 'interacted' with.
	//i.e. if the player ovelaps with the object and presses up if they will
	//talk to or enter the object.
	virtual bool can_interact_with() const { return false; }

protected:
	void set_current_generator(current_generator* generator);

	void set_respawn(bool value) { respawn_ = value; }

	//move the entity by a number of centi pixels.
	void move_centipixels(int x, int y) { x_ += x; y_ += y; }

	void set_editor_info(const_editor_entity_info_ptr p) { editor_info_ = p; }

private:
	virtual void control(const level& lvl) = 0;

	std::string label_;

	int x_, y_;

	bool face_right_;
	bool upside_down_;

	//the entity group the entity is in.
	int group_;

	int id_;

	bool respawn_;

	const_editor_entity_info_ptr editor_info_;

	current_generator_ptr current_generator_;
};

#endif
