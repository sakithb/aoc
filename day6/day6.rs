use std::fs;

#[derive(Clone, Copy, Debug)]
enum Direction {
    Top,
    Right,
    Bottom,
    Left
}

#[derive(Clone, Copy, Debug)]
enum Cell {
    Empty,
    Barrier,
    Guard(Direction),
}

fn main() {
    let input = fs::read_to_string("day6_input.txt").unwrap();
    let mut map: Vec<Vec<Cell>> = input
        .lines()
        .map(|l| l.chars().map(|c| {
            if c == '.' {
                Cell::Empty
            } else if c == '#' {
                Cell::Barrier
            } else if c == '^' {
                Cell::Guard(Direction::Top)
            } else if c == '>' {
                Cell::Guard(Direction::Right)
            } else if c == 'v' {
                Cell::Guard(Direction::Bottom)
            } else if c == '<' {
                Cell::Guard(Direction::Left)
            } else {
                panic!("Huh");
            }
        }).collect())
        .collect();

    let w = map[0].len();
    let h = map.len();

    let mut gx = w;
    let mut gy = h;

    let mut x = 0;
    let mut y = 0;

    while y < h && gy == h {
        while x < w && gx == w {
            match map[y][x] {
                Cell::Guard(_) => {
                    gx = x;
                    gy = y;
                },
                _ => (),
            }

            x += 1;
        }

        y += 1;
        x = 0;
    }

    loop {
        let Cell::Guard(mut direction) = map[gy][gx] else { panic!() };
        let nc: Cell = match direction {
            Direction::Top => if gy == 0 { break } else { map[gy-1][gx] },
            Direction::Right => if gx == w - 1 { break } else { map[gy][gx+1] },
            Direction::Bottom => if gy == h - 1 { break } else { map[gy+1][gx] },
            Direction::Left => if gx == 0 { break } else { map[gy][gx-1] },
        };

        match nc {
            Cell::Barrier => {
                let nd = match direction {
                    Direction::Top => Direction::Right,
                    Direction::Right => Direction::Bottom,
                    Direction::Bottom => Direction::Left,
                    Direction::Left => Direction::Top,
                };

                match nd {
                    Direction::Top => gy -= 1,
                    Direction::Right => gx += 1,
                    Direction::Bottom => gy += 1,
                    Direction::Left => gx -= 1,
                }

                direction = nd;
            },
            _ => {
                match direction {
                    Direction::Top => gy -= 1,
                    Direction::Right => gx += 1,
                    Direction::Bottom => gy += 1,
                    Direction::Left => gx -= 1,
                }
            }
        }

        map[gy][gx] = Cell::Guard(direction);
    }

    let mut count = 0;

    for y in 0..h {
        for x in 0..w {
            match map[y][x] {
                Cell::Guard(_) => count += 1,
                _ => ()
            };
        }
    }

    println!("{}", count);
}
