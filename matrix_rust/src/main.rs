use std::io::{self, Read};

// 행렬을 Vec<Vec<i32>> 로 저장한다.
fn read_matrix(it: &mut impl Iterator<Item = i32>, rows: usize, cols: usize) -> Vec<Vec<i32>> {
    let mut m = Vec::with_capacity(rows);
    for _ in 0..rows {
        let mut row = Vec::with_capacity(cols);
        for _ in 0..cols {
            row.push(it.next().expect("입력이 부족합니다"));
        }
        m.push(row);
    }
    m
}

fn add(a: &Vec<Vec<i32>>, b: &Vec<Vec<i32>>) -> Vec<Vec<i32>> {
    let mut c = Vec::new();
    for i in 0..a.len() {
        let mut row = Vec::new();
        for j in 0..a[i].len() {
            row.push(a[i][j] + b[i][j]);
        }
        c.push(row);
    }
    c
}

fn print_matrix(name: &str, m: &Vec<Vec<i32>>) {
    println!("{}:", name);
    for row in m {
        for v in row {
            print!("{:4}", v);
        }
        println!();
    }
}

fn main() {
    // 첫 줄에 행, 열 크기를 입력받고 이어서 두 행렬의 원소를 입력받는다.
    let mut input = String::new();
    io::stdin().read_to_string(&mut input).unwrap();
    let mut it = input.split_whitespace().map(|x| x.parse::<i32>().unwrap());

    let rows = it.next().unwrap() as usize;
    let cols = it.next().unwrap() as usize;

    let a = read_matrix(&mut it, rows, cols);
    let b = read_matrix(&mut it, rows, cols);
    let c = add(&a, &b);

    print_matrix("A", &a);
    print_matrix("B", &b);
    print_matrix("A + B", &c);
}
