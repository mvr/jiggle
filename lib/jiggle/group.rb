module Jiggle
  
  class Group
    attr_reader :particles
    attr_reader :springs
    attr_reader :areas

    def initialize
      @particles = []
      @springs = []
      @areas = []
    end

    def points
      particles.map { |p| p.position }
    end
    
    def objects
      [@particles, @springs, @areas].flatten
    end

    def contains?(thing)
      @areas.any? {|a| a.contains? point}
    end

    def friction=(value)
      [@particles, @areas].flatten.each do |p|
        p.friction = value
      end
    end
    
    def expand(amount = 2)
      @springs.each do |spring|
        spring.length *= amount
      end
    end
    def contract(amount = 2)
      @springs.each do |spring|
        spring.length /= amount
      end
    end

    def center_of_mass
      @areas[0].center_of_mass
    end
    
  end

  class Space
    def add_group(group)
      group.objects.each do |o|
        add o
      end
    end
    
    def remove_group(group)
      group.objects.each do |o|
        remove o
      end
    end
  end
  
end
