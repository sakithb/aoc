use std::fs;

#[derive(Debug)]
enum Op {
    MUL,
    ADD,
    CON
}

fn format_radix(mut x: u32, radix: u32) -> String {
    let mut result = vec![];

    loop {
        let m = x % radix;
        x = x / radix;

        result.push(std::char::from_digit(m, radix).unwrap());
        if x == 0 {
            break;
        }
    }

    let l = 64 - result.len();
    for _ in 0..l {
        result.push('0');
    }

    result.into_iter().rev().collect()
}

fn num_to_ops(num: u64, count: usize) -> Vec<Op> {
    let bs = format_radix(num as u32, 3 as u32);
    let st = 64 - count;

    bs[st..]
        .chars()
        .map(|c| {
            match c {
                '0' => Op::MUL,
                '1' => Op::ADD,
                _ => Op::CON,
            }
        })
        .collect()
}

fn main() {
    let input = fs::read_to_string("day7_input.txt").unwrap();
    let eqs: Vec<(u64, Vec<u64>)> = input
        .lines()
        .map(|l| {
            let (result, operands) = l.split_once(": ").unwrap();
            (result.parse::<u64>().unwrap(), operands.split(" ").map(|o| o.parse::<u64>().unwrap()).collect::<Vec<u64>>())})
        .collect();

    let mut sum = 0;

    'outer: for (result, operands) in eqs {
        let op_count = operands.len() - 1;
        let op_num = u64::pow(3 as u64, op_count as u32);

        for i in 0..op_num {
            let mut ops: Vec<Op> = num_to_ops(i, op_count).into_iter().rev().collect();
            let r = operands.clone().into_iter().reduce(|acc, e| {
                let op = ops.pop().unwrap();
                match op {
                    Op::MUL => acc * e,
                    Op::ADD => acc + e,
                    Op::CON => {
                        let mut s = acc.to_string();
                        s.push_str(&e.to_string());
                        s.parse::<u64>().unwrap()
                    }
                }
            }).unwrap();

            if r == result {
                sum += result;
                continue 'outer;
            }
        }
    }

    println!("{}", sum);
}
