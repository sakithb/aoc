use std::collections::HashMap;
use std::collections::HashSet;
use std::cmp::Ordering;
use std::fs;

fn sep_rule(s: &str) -> (u32, u32) {
    let x = s.split_once("|").unwrap();
    (x.0.parse().unwrap(), x.1.parse().unwrap())
}

fn main() {
    let input = fs::read_to_string("day5_input.txt").unwrap();
    let sections = input.trim().split_once("\n\n").unwrap();

    let mut rules: HashMap<i32, HashSet<i32>> = HashMap::new();
    for (k, v) in sections.0.lines().map(|line| sep_rule(line)) {
        rules.entry(k as i32).or_insert_with(HashSet::new).insert(v as i32);
    }

    let mut updates = sections.1
        .lines()
        .map(|line| line
            .split(",")
            .map(|n| n .parse::<i32>() .unwrap())
            .collect::<Vec<i32>>())
        .collect::<Vec<Vec<i32>>>();
        
    let sum = updates
        .iter_mut()
        .filter(|update| !update.is_sorted_by(|a, b| if rules.contains_key(a) { rules[a].contains(b) } else { false }))
        .map(|update| {
            update.sort_by(|a, b| if rules.contains_key(a) && rules[a].contains(b) { Ordering::Less } else { Ordering::Greater });
            update[update.len() / 2]
        })
        .sum::<i32>();

    println!("{}", sum);
}
