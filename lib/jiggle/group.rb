module Jiggle
  
  class Group
    attr_accessor :particles
    attr_accessor :springs
    attr_accessor :areas

    def initialize
      @particles = []
      @springs = []
      @areas = []
    end

    def contains?(thing)
      @areas.any? {|a| a.contains? point}
    end

    def elasticity=(value)
      [@particles, @areas].flatten.each do |p|
        p.elasticity = value
      end
    end
  end

  class World
    def add_group(group)
      group.particles.each do |particle| 
        add_particle particle
      end
      
      group.springs.each do |spring| 
        add_spring spring
      end

      group.areas.each do |area| 
        add_area area
      end
    end
    
    def remove_group(group)
      group.particles.each do |particle| 
        remove_particle particle
      end
      
      group.springs.each do |spring| 
        remove_spring spring
      end

      group.areas.each do |area|
        remove_area area
      end
    end
  end
  
end
