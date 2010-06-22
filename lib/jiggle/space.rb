module Jiggle

  class Space
    attr_reader :particles
    attr_reader :springs
    attr_reader :areas
    
    def setup
      @time_step = 1 / @ticks_per_second
      @time_accumulator = 0
    end
    private :setup
    
    def update(timestep)
      clear_collisions

      @time_accumulator += timestep
      
      while @time_accumulator > @time_step
        step(@time_step)
        @time_accumulator -= @time_step
      end
    end
    
    def add(thing)
      case thing
      when Particle
        add_particle thing
      when Spring
        add_spring thing
      when Area
        add_area thing
      when Group
        add_group thing
      end
    end
    alias :<< :add
    
    def remove(thing)
      case thing
      when Particle
        remove_particle thing
      when Spring
        remove_spring thing
      when Area
        remove_area thing
      when Group
        remove_group thing
      end
    end
  end
  
end

  
