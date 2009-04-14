#!/usr/bin/ruby

require 'lib/jiggle'
include Jiggle
require 'rubygems'
require 'rubygame'
include Rubygame

class Vector2
  def to_world
    (self - jgv(200, 200)) / 5
  end
  def to_screen
    (self * 5) + jgv(200, 200)
  end
end

@start_time = Time.now

def current_time
  ((Time.now - @start_time).to_f)
end

Rubygame.init
screen = Screen.set_mode [400, 400]
queue = EventQueue.new()

w = World.new
w.gravity = jgv(0, 98)


box_shape = [jgv(0, 0),
             jgv(0, 10),
             jgv(10, 10),
             jgv(10, 0)]

#b = StaticBody.new box_shape
#w.add_body b

r = Rope.new jgv(-20, -30), jgv(20, -30), :number_of_points => 30, :mass => 10
w.add_rope r

buffer = Surface.new(screen.size)

loop do
  queue.each do |event|
    case(event)
    when MouseDownEvent
      position = jgv(*event.pos).to_world
      b = Body.new box_shape, :offset => position
      w.add_body b
    end
  end
  
  buffer.fill :black
  
  w.particles.each do |particle|
    position = particle.position.to_screen
    buffer.draw_circle_s [position.x, position.y], 2, :white
  end

  w.springs.each do |spring|
    p_a = spring.a.position.to_screen
    p_b = spring.b.position.to_screen
    buffer.draw_line [p_a.x, p_a.y], [p_b.x, p_b.y], :blue
  end

  w.areas.each do |area|
    area.particles.adjacent.each do |a, b|
      p_a = a.position.to_screen
      p_b = b.position.to_screen
      buffer.draw_line [p_a.x, p_a.y], [p_b.x, p_b.y], :red
    end
  end
  
  buffer.blit(screen, [0,0])
  w.update(current_time)
  screen.update
end

Rubygame.quit
