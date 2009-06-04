require 'jiggle/group'
require 'matrix' # I'm lazy.

# OH MY EYES BURN

module Jiggle

  module BlobMatrix
    def rows
      self # Ha.
    end

    def columns
      result = []
      size.times do |idx|
        result << self.map do |row|
          row[idx]
        end
      end
      result
    end

    def size
      self[0].size
    end
  end
  
  class QuadBlob < Group
    def initialize(quad, attr={})
      super()

      # Horrible horrible duplication
      grid_number = attr[:grid_number] || 4
      
      attr[:mass] ||= 4.0
      mass = attr[:mass] / (grid_number ** 2)

      scale  = attr[:scale]  || 1
      offset = attr[:offset] || jgv(0, 0)
      
      quad = quad.map { |point| (point * scale) + offset }

      first_column = points_along(quad[0], quad[1], grid_number)
      last_column = points_along(quad[3], quad[2], grid_number)
      
      matrix = first_column.zip(last_column).map do |a, b|
        points_along(a, b, grid_number)
      end

      matrix.map! do |row|
        row.map! do |point|
          Particle.new point, :mass => mass
        end
      end
      
      @particles = matrix.flatten
      
      matrix.send(:extend, BlobMatrix)
      
      # Row and column springs
      (matrix.rows + matrix.columns).each do |row|
        row.adjacent(false).each do |p1, p2|
          @springs << Spring.new(p1, p2, attr)
        end
      end

      # Crossing springs
      (matrix.size - 1).times do |row_idx|
        row = matrix[row_idx]
        next_row = matrix[row_idx + 1]
        matrix.size.times do |column_idx|
          particle = row[column_idx]
          @springs << Spring.new(particle,
                                 next_row[column_idx - 1],
                                 attr) if column_idx > 0
          @springs << Spring.new(particle,
                                 next_row[column_idx + 1],
                                 attr) if column_idx < matrix.size - 1
        end
      end

      # Corner to corner springs
      [matrix.rows.first.first,
       matrix.rows.last.last,
       matrix.rows.first.last,
       matrix.rows.last.first].pairs.each do |a, b|
        @springs << Spring.new(a, b, attr)
      end
      
      edge_particles = ( matrix.rows.first +
                         matrix.columns.last[1..-1] +
                         matrix.rows.last.reverse[1..-1] +
                         matrix.columns.first.reverse[1..-2] ).reverse
      interior_particles = (matrix.flatten - edge_particles)
      
      @areas << Area.new(edge_particles)
      @edge_particles = edge_particles
      
      interior_particles.each do |particle|
        particle.collidable = false
      end
    end
    
    def particles
      @edge_particles
    end
    
    protected
    
    # More duplication AH.
    def points_along(a, b, num)
      step = (b - a) / (num - 1)
      points = []
      num.times do |i|
        points << a + (step * i)
      end
      points
    end
  end
  
end
