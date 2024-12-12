use std::fs;

#[derive(Debug)]
enum Op {
    MUL,
    ADD
}

fn num_to_ops(num: u64, count: usize) -> Vec<Op> {
    let bs = format!("{:064b}", num);
    let st = 64 - count;

    bs[st..]
        .chars()
        .map(|c| if c == '0' { Op::MUL } else { Op::ADD })
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
        let op_num = u64::pow(2 as u64, op_count as u32);

        for i in 0..op_num {
            let mut ops: Vec<Op> = num_to_ops(i, op_count).into_iter().rev().collect();
            let r = operands.clone().into_iter().reduce(|acc, e| {
                let op = ops.pop().unwrap();
                match op {
                    Op::MUL => acc * e,
                    Op::ADD => acc + e,
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
