pub struct UserId(String);

impl UserId {
    pub fn new(id: String) -> UserId {
        UserId(id)
    }

    pub fn to_string(&self) -> String {
        String::from(&self.0)
    }
}
