use std::io;

fn main() {
    let mut input = String::new();
    io::stdin().read_line(&mut input).expect("Failed to read line");
    let result = input.trim().parse::<u32>();
    if result.is_ok()
    {
        let num = result.unwrap();
        let mut candidate = num - 1;
        let mut prime = true;
        while candidate > 1
        {
            if num % candidate == 0
            {
                prime = false;
                break;
            }
            candidate = candidate - 1;
        }
        if prime
        {
            println!("{} is prime", num);
        }
        else 
        {
            println!("{} is not prime", num);
        }
    }
}
