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

w = World.new
w.gravity = jgv(0, 9.8)

p1 = Particle.new jgv(10, 0)
w.add_particle p1

p2 = Particle.new jgv(0, 0)
w.add_particle p2
p2.mass = 5

s = Spring.new p1, p2, :length => 5
w.add_spring s

buffer = Surface.new(screen.size)

loop do
  buffer.fill :black
  
  w.particles.each do |particle|
    position = particle.position.to_screen
    p position
    buffer.draw_circle_s [position.x, position.y], 2, :white
  end

  w.springs.each do |spring|
    p_a = spring.a.position.to_screen
    p_b = spring.b.position.to_screen
    buffer.draw_line [p_a.x, p_a.y], [p_b.x, p_b.y], :white
  end
  
  buffer.blit(screen, [0,0])
  w.update(current_time)
  screen.update
end

Rubygame.quit
