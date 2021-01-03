pub struct BoardId(String);

impl BoardId {
    pub fn new(id: String) -> BoardId {
        BoardId(id)
    }

    pub fn to_string(&self) -> String {
        String::from(&self.0)
    }
}
