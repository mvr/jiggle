module Jiggle
  class Body
    attr_accessor :particles
    attr_accessor :springs
    attr_accessor :area

    def initialize(points, attr={})
      attr[:mass] ||= points.size
      
      mass = attr[:mass] / points.size

      @particles = []
      points.each do |point|
        particle = Particle.new point, :mass => mass
        @particles << particle
      end

      @springs = []
      @particles.pairs.each do |p1, p2|
        spring = Spring.new p1, p2, :strength => attr[:strength], :damping => attr[:damping]
        @springs << spring
      end

      @area = Area.new @particles
    end
  end
  
  class World
    def add_body(body)
      body.particles.each do |particle| 
        add_particle particle
      end
      
      body.springs.each do |spring| 
        add_spring spring
      end
      
      add_area body.area
    end
    
    def remove_body(body)
      body.particles.each do |particle| 
        remove_particle particle
      end
      
      body.springs.each do |spring| 
        remove_spring spring
      end
      
      remove_area body.area
    end
  end
end
