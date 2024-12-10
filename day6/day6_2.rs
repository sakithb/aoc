use std::fs;

#[derive(Clone, Copy, Debug, PartialEq)]
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

fn print_map(map: &Vec<Vec<Cell>>, w: usize, h: usize) {
    println!("------------------------------------------------");
    for y in 0..h {
        for x in 0..w {
            print!("{}", match map[y][x] {
                Cell::Empty => '.',
                Cell::Barrier => '#',
                Cell::Guard(d) => match d {
                    Direction::Top => '^',
                    Direction::Left => '<',
                    Direction::Right => '>',
                    Direction::Bottom => 'v'
                }
            });
        }
        print!("\n");
    }

    println!("------------------------------------------------");
}

fn check_loop(smap: &Vec<Vec<Cell>>, sgx: usize, sgy: usize, w: usize, h: usize) -> bool {
    let mut gx = sgx;
    let mut gy = sgy;

    let mut map = smap.clone();
    let mut visited: Vec<(usize, usize, Direction)> = Vec::new();

    loop {
        let Cell::Guard(mut direction) = map[gy][gx] else { panic!() };

        let mut nx = gx;
        let mut ny = gy;

        match direction {
            Direction::Top => if gy == 0 { return false } else { ny -= 1 },
            Direction::Right => if gx == w - 1 { return false } else { nx += 1 },
            Direction::Bottom => if gy == h - 1 { return false } else { ny += 1 },
            Direction::Left => if gx == 0 { return false } else { nx -= 1 },
        };

        let new_cell = map[ny][nx];

        match new_cell {
            Cell::Barrier => {
                let new_direction = match direction {
                    Direction::Top => Direction::Right,
                    Direction::Right => Direction::Bottom,
                    Direction::Bottom => Direction::Left,
                    Direction::Left => Direction::Top,
                };

                direction = new_direction;

                let v = (gx, gy, direction);
                if visited.contains(&v) {
                    return true;
                } else {
                    visited.push(v);
                }
            },
            _ => {
                map[gy][gx] = Cell::Empty;

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
    
    let mut count = 0;

    y = 0;
    while y < h {
        x = 0;
        while x < w {
            let c: Cell = map[y][x];
            match c {
                Cell::Empty => {
                    map[y][x] = Cell::Barrier;
                    if check_loop(&map, gx, gy, w, h) {
                        count += 1;
                        // print_map(&map, w, h);
                    }
                    map[y][x] = Cell::Empty;
                },
                _ => ()
            }

            x += 1;
        }
        y += 1;
    }
    
    println!("{}", count);
}
