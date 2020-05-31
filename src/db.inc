<?php
class db
{
    private function connect()
    {
        $userName = "karim";
        $pass = '';
        $host = 'localhost';
        $type = 'mysql';
        $dbname = 'sakila';
        try {
            $pdo = new PDO($type . ":host=" . $host . ";dbname=" . $dbname . ";", $userName, '' . $pass);
            $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
            return $pdo;
        } catch (PDOException $e) {
            echo $e;
            echo '<br>';}
    }

    public function query(String $query, $where = null, $fetch = 2)
    {
        try {
            $keyWord = array("SHOW", "DESCRIBE", "SELECT", "INSERT", "DELETE", "UPDATE","show", "describe", "select", "insert", "delete", "update");
            $sql = (in_array(explode(" ", $query)[0], $keyWord)) ? $query : "select * from " . $query;
            if ($where !=null) {
                $sql= $sql . " WHERE ".$where;
            }
            
            $pdo = $this->connect();
            $stmt = $pdo->query($sql);
            return json_encode($stmt->fetchAll($fetch));
        } catch (Exception $e) {
            echo $sql;
            echo $e;
            echo '<br>';}
    }

    public function insert($table, $data)
    {
        try {
            $data_array = (is_array($data)) ? $data : json_decode(str_replace("'", '"', $data), true);
            $sql = 'INSERT INTO ' . $table . "(`" . join("`,`", array_keys($data_array)) . "`) values ('" . join("','", array_values($data_array)) . "');";
            $this->connect()->query($sql);
            $where = implode(' and ', array_map(
                function ($v, $k) {return sprintf("`%s`='%s'", $k, $v);},
                $data_array,
                array_keys($data_array)));
            return $this->query($table, $where, 2);

        } catch (Exception $e) {
            echo $e;
            echo '<br>';
        }
    }
    public function update($table, $data, $where)
    {
        try {
            
            $DatajsonString = str_replace('}', '', str_replace('" :', '`=', str_replace('":', '`=', str_replace(', "', ',`', str_replace(',"', ',`', str_replace('{ "', '`', str_replace('{"', '`',json_encode($data))))))));
            $sql = 'update ' . $table . ' set ' . $DatajsonString . ' where ' . $where;
            $this->connect()->query($sql);
            return $this->query($table, $where);

        } catch (Exception $e) {
            echo $e;
            echo '<br>';
        }
    }

    public function delete($table, $where)
    {
        try {
            $sql = 'delete from ' . $table . ' where ' . $where;
            $pdo = $this->connect();
            $stmt = $pdo->query($sql);
            return  $stmt->rowCount();
        } catch (PDOException $e) {
            echo $e;
            echo '<br>';
            echo $sql;
        }
    }
}
