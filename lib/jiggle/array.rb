class Array
  def adjacent(include_first_and_last = true)
    pairs = []
    idx = 0
    while idx < self.length - 1
      pairs << [self[idx], self[idx + 1]]
      idx += 1
    end

    pairs << [self[0], self[length - 1]] if include_first_and_last and self.length > 2
    pairs
  end

  def pairs
    combos = []
    i = 0
    while i < self.length
      i2 = i + 1
      while i2 < self.length
        combos << [self[i], self[i2]]
        i2 += 1
      end
      i += 1
    end
    combos
  end
end
