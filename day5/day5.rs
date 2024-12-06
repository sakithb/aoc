use std::fs;

fn main() {
    let input = fs::read_to_string("day5_input.txt").unwrap();
    let sections: Vec<&str> = input.trim().split("\n\n").collect();
    let rules_str: Vec<&str> = sections[0].split("\n").collect();
    let updates_str: Vec<&str> = sections[1].split("\n").collect();

    let mut rules: Vec<(i32, i32)> = Vec::new();

    for rule_str in rules_str {
        let ns: Vec<i32> = rule_str.split("|").map(|i| i.parse::<i32>().unwrap()).collect();
        rules.push((ns[0], ns[1]));
    }

    let mut sum: i32 = 0;

    for update_str in updates_str {
        let mut is_valid: bool = true;
        let ns: Vec<i32> = update_str.split(",").map(|i| i.parse::<i32>().unwrap()).collect();
        for (i, n) in ns.iter().enumerate() {
            for (j, m) in ns.iter().enumerate() {
                if j <= i { continue }; 
                for (f, l) in rules.iter() {
                    if *n == *l && *m == *f {
                        is_valid = false;
                    }
                }
            }
        }

        if is_valid {
            sum += ns[ns.len() / 2];
        }
    }

    println!("{}", sum);
}
